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

    // Player window
    PlayerWindow player;
    player.setWindowTitle("Qt Video Player");
    player.show();
    player.resize(1280, 720);

    ControlsWidget controls(&player);
    controls.setWindowTitle("Controls");
    controls.show();
    controls.resize(1280, 500);

    return app.exec();
}