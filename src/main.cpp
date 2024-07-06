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

    // Player window
    PlayerWindow player;
    QWidget playerWindow;
    QVBoxLayout *layout = new QVBoxLayout;

    player.setParent(&playerWindow);
    layout->addWidget(&player);
    layout->setContentsMargins(0, 0, 0, 0);
    playerWindow.setLayout(layout);

    // Debug
    
    playerWindow.setWindowTitle("Qt Video Player");
    playerWindow.show();
    playerWindow.resize(1280, 720);

    // player.openFile();

    // Controls window
    ControlsWidget controls(&player);
    QWidget controlsWindow;
    QVBoxLayout *controlsLayout = new QVBoxLayout;

    controls.setParent(&controlsWindow);
    controlsLayout->addWidget(&controls);
    controlsLayout->setContentsMargins(0, 0, 0, 0);
    controlsWindow.setLayout(controlsLayout);

    controlsWindow.setWindowTitle("Controls");
    controlsWindow.show();
    controlsWindow.resize(1280, 100);

    return app.exec();
}