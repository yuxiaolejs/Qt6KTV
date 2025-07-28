#ifndef LOGINWIDGET_HPP
#define LOGINWIDGET_HPP

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QStyle>
#include <QHBoxLayout>
#include <QAudioOutput>
#include <QCloseEvent>
#include <QNetworkAccessManager>

#include "playerWindow.hpp"
#include "constants.hpp"

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    LoginWidget();
    ~LoginWidget();
private:
    void login(const QString &auth);
    QNetworkAccessManager* manager;

signals:
    void loginOK(QString auth);
};

#endif