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
#include <QLabel>

#include <iostream>
#include "playerWindow.hpp"
#include "mediaWidget.hpp"
#include "controlsWidget.hpp"
#include "queueWidget.hpp"
#include "loginWidget.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyleSheet(R"(
        QWidget {
            font-family: "Segoe UI";
            font-size: 14px;
        }
        QPushButton {
            background-color: #2ecc71;
            color: white;
            border-radius: 5px;
            padding: 5px 10px;
        }
        QPushButton:hover {
            background-color: #27ae60;
        }
    )");
    LoginWidget loginWidget;
    loginWidget.setWindowTitle("Login");
    loginWidget.resize(300, 200);
    loginWidget.show();

    // Connect login signal to initialize main application
    QObject::connect(&loginWidget, &LoginWidget::loginOK, [&](QString auth)
                     {
    loginWidget.close();
    PlayerWindow* player = new PlayerWindow();
    player->setWindowTitle("Qt Video Player");
    player->show();
    player->resize(1280, 720);

    ControlsWidget* controls = new ControlsWidget(player);
    controls->setWindowTitle("Controls");
    controls->show();
    controls->resize(1280, 500); });
    return app.exec();
}