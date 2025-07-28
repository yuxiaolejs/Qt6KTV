#include "loginWidget.hpp"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
LoginWidget::LoginWidget()
{
    // Create a login form with username and password fields
    manager = new QNetworkAccessManager(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLineEdit *usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Username");
    QLineEdit *passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Password");
    passwordField->setEchoMode(QLineEdit::Password);
    QPushButton *loginButton = new QPushButton("Login", this);
    layout->addWidget(usernameField);
    layout->addWidget(passwordField);
    layout->addWidget(loginButton);
    setLayout(layout);

    // When button is clicked, send login request
    connect(loginButton, &QPushButton::clicked, this, [=]() {
        QString username = usernameField->text();
        QString password = passwordField->text();
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
            return;
        }
        QString auth = username + ":" + password;
        login(auth);
    });
}

LoginWidget::~LoginWidget()
{
    delete manager;
}

void LoginWidget::login(const QString &auth)
{
    QUrl url(QString(BASE_URL) + "/api/v1/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", auth.toUtf8());

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]()
            {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Network error:" << reply->errorString();
            qDebug() << "Received response from server:" << reply->errorString();
            QMessageBox::information(nullptr, "Error", "Network error: " + reply->errorString());
            return;
                }
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if (statusCode.isValid()) {
            int httpStatus = statusCode.toInt();
            qDebug() << "HTTP status code:" << httpStatus;
            if(httpStatus != 200) {
                qDebug() << "Failed to download file, HTTP status code:" << httpStatus;
                QMessageBox::information(nullptr, "Error", "Could not download file, HTTP status code: " + QString::number(httpStatus));
                return;
            }
        }
        emit loginOK(auth);
    reply->deleteLater(); });
}