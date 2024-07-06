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

    // QLabel *label = new QLabel("Hello World", &window);
    // layout->addWidget(label);

    player.setParent(&window);

    layout->addWidget(&player);
    window.setLayout(layout);

    // Debug
    


    window.setWindowTitle("Qt Video Player");
    window.show();

    player.openFile();

    return app.exec();
}