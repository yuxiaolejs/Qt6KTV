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
#include <cmath>

#include "controlsWidget.hpp"

ControlsWidget::ControlsWidget(PlayerWindow *target) : QWidget()
{
    this->target = target;
    QPushButton *openButton = new QPushButton("Open Video", this);
    QObject::connect(openButton, &QPushButton::clicked, [=]()
                     { target->openFile(); });

    // Create play button
    QPushButton *playButton = new QPushButton();
    playButton->setIcon(playButton->style()->standardIcon(QStyle::SP_MediaPause));
    QObject::connect(playButton, &QPushButton::clicked, [=]()
                     { target->playVideo(playButton); });

    // Create play button
    QPushButton *audSwitch = new QPushButton();
    audSwitch->setIcon(audSwitch->style()->standardIcon(QStyle::SP_VistaShield));
    QObject::connect(audSwitch, &QPushButton::clicked, [=]()
                     { target->switchAudio(); });

    // Create slider for video progress
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    QObject::connect(slider, &QSlider::sliderMoved, [=](float position)
                     { 
                        float vol = 1-log10(101-(float)position) / 2;
                        target->setVolume(vol);
                     std::cout << "Set volume "<< vol << std::endl; });

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(audSwitch);
    controlLayout->addWidget(slider);
    this->setLayout(controlLayout);
}

void ControlsWidget::closeEvent(QCloseEvent *event) {
    std::cout << "ControlsWidget closed" << std::endl;
    target->close();
    event->accept();  // Accept the close event to allow the widget to close
}