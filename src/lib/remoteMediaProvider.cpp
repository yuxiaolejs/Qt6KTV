#include "remoteMediaProvider.hpp"
#include <QDir>
#include <QFileInfoList>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QUrlQuery>
#include <QMessageBox>

RemoteMediaProvider::RemoteMediaProvider(QString basePath)
{
    manager = new QNetworkAccessManager();
    this->basePath = basePath;
}
RemoteMediaProvider::~RemoteMediaProvider()
{
    delete manager;
}
QStringList RemoteMediaProvider::listMedia()
{
    return QStringList();
}
QStringList RemoteMediaProvider::searchMedia(const QString &query)
{
    QUrl url(basePath + "/api/v1/media/search");
    QUrlQuery queryParams;
    queryParams.addQueryItem("query", query);
    url.setQuery(queryParams);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", authToken.toUtf8());

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]()
            {
        QStringList allItems;
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
        QByteArray response = reply->readAll();
        qDebug() << "Response data:" << response;
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (doc.isArray()) {
            QJsonArray obj = doc.array();
            qDebug() << "JSON response:" << obj;
            for (const QJsonValue &value : obj) {
                QString o = value.toString();
                allItems.append(o);
            }
            emit mediaListUpdated(allItems);
        }
    reply->deleteLater(); });
    return QStringList();
}
QString RemoteMediaProvider::getLocalMediaPath(const QString &mediaName)
{
    // Check if local "mediaCache" directory exists
    QDir mediaCacheDir(QCoreApplication::applicationDirPath() + "/mediaCache");
    if (!mediaCacheDir.exists())
        mediaCacheDir.mkpath(".");
    // Check if the file already exists
    QString localFilePath = mediaCacheDir.filePath(mediaName);
    if (QFile::exists(localFilePath))
    {
        qDebug() << "File already exists locally:" << localFilePath;
        emit localMediaPathReady(localFilePath);
        return localFilePath;
    }
    // If not, download the file
    QUrl url(basePath + "/api/v1/media/" + mediaName);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setRawHeader("Authorization", authToken.toUtf8());
    QNetworkReply *reply = manager->get(request);

    QFile *file = new QFile(localFilePath);
    bool *fileExists = new bool(true);
    if (!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open file for writing:" << file->errorString();
        QMessageBox::information(nullptr, "Error", "Failed to open file for writing: " + file->errorString());
        delete file;
        return QString();
    }

    connect(reply, &QNetworkReply::metaDataChanged, this, [=]()
            {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Network error:" << reply->errorString();
            *fileExists = false;
            QMessageBox::information(nullptr, "Error", "Network error: " + reply->errorString());
            return;
        }
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if (statusCode.isValid()) {
            int httpStatus = statusCode.toInt();
            qDebug() << "HTTP status code:" << httpStatus;
            if(httpStatus != 200) {
                qDebug() << "Failed to download file, HTTP status code:" << httpStatus;
                *fileExists = false;
                QMessageBox::information(nullptr, "Error", "Could not download file, HTTP status code: " + QString::number(httpStatus));
                return;
            }
        } });

    connect(reply, &QNetworkReply::readyRead, [=]()
            { 
                // if not open, return;
        if (!*fileExists || !file->isOpen()) {
            qDebug() << "File is not open for writing.";
            return;
        }
                file->write(reply->readAll()); });
    connect(reply, &QNetworkReply::finished, [=]()
            {
        if(*fileExists)
        {
            file->flush();
            file->close();
            delete file;
            qDebug() << "Download finished";
            emit localMediaPathReady(localFilePath);
        }else{
            qDebug() << "Download failed, file not saved.";
            // remove file if it exists
            if (QFile::exists(localFilePath)) {
                QFile::remove(localFilePath);
            }
            QMessageBox::information(nullptr, "Error", "Download failed, file not saved.");
        }
        delete fileExists;
        reply->deleteLater(); });
    return localFilePath;
}
