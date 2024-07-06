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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PlayerWindow player;
    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout;

    player.setParent(&window);
    layout->addWidget(&player);
    layout->setContentsMargins(0, 0, 0, 0);
    window.setLayout(layout);

    // Debug
    
    window.setWindowTitle("Qt Video Player");
    window.show();
    window.setFixedSize(1280, 720);

    player.openFile();

    return app.exec();
}