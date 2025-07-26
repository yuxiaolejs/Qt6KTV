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
#include "controlsWidget.hpp"

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
    // QWidget playerWindow;
    // QVBoxLayout *layout = new QVBoxLayout;

    // player.setParent(&playerWindow);
    // layout->addWidget(&player);
    // layout->setContentsMargins(0, 0, 0, 0);
    // playerWindow.setLayout(layout);

    // Debug
    
    player.setWindowTitle("Qt Video Player");
    player.show();
    player.resize(1280, 720);

    // player.openFile();

    // Controls window
    ControlsWidget controls(&player);
    // QWidget controlsWindow;
    // QVBoxLayout *controlsLayout = new QVBoxLayout;

    // controls.setParent(&controlsWindow);
    // controlsLayout->addWidget(&controls);
    // controlsLayout->setContentsMargins(0, 0, 0, 0);
    // controlsWindow.setLayout(controlsLayout);

    controls.setWindowTitle("Controls");
    controls.show();
    controls.resize(1280, 100);

    return app.exec();
}