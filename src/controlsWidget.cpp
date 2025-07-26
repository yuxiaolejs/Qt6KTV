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
#include "mediaWidget.hpp"
#include "queueWidget.hpp"

ControlsWidget::ControlsWidget(PlayerWindow *target) : QWidget()
{
    this->target = target;
    QueueWidget *queueWidget = new QueueWidget(this);
    MediaWidget *mediaWidget = new MediaWidget(this, queueWidget);

    QPushButton *nextButton = new QPushButton("Next", this);
    QObject::connect(nextButton, &QPushButton::clicked, [=]()
                     { target->switchToEnd(); });

    // Create play button
    QPushButton *playButton = new QPushButton();
    playButton->setIcon(playButton->style()->standardIcon(QStyle::SP_MediaPause));
    QObject::connect(playButton, &QPushButton::clicked, [=]()
                     { 
                        target->playVideo(playButton); });

    QPushButton *audSwitch = new QPushButton();
    audSwitch->setIcon(audSwitch->style()->standardIcon(QStyle::SP_VistaShield));
    QObject::connect(audSwitch, &QPushButton::clicked, [=]()
                     { target->switchAudio(); });

    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    QObject::connect(slider, &QSlider::sliderMoved, [=](float position)
                     { 
                        float vol = 1-log10(101-(float)position) / 2;
                        target->setVolume(vol);
                     std::cout << "Set volume "<< vol << std::endl; });

    // Control components: media and queue
    QHBoxLayout *mediaLayout = new QHBoxLayout;
    mediaLayout->addWidget(mediaWidget);
    mediaLayout->addWidget(queueWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(nextButton);
    controlLayout->addWidget(audSwitch);
    controlLayout->addWidget(slider);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(mediaLayout);
    this->setLayout(mainLayout);

    // Register events for media player
    auto player = target->mediaPlayer;
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status)
            {
        qDebug() << ">>>Media status changed:" << status;
        if (status == QMediaPlayer::EndOfMedia){
            // load next media from the queue
            QString mediaFile = queueWidget->get();
            target->openFile(mediaFile);
        }
        switch (status) {
        case QMediaPlayer::NoMedia:
            std::cout << "No media loaded." << std::endl;
            break;
        case QMediaPlayer::LoadingMedia:
            std::cout << "Loading media..." << std::endl;
            break;
        case QMediaPlayer::LoadedMedia:
            std::cout << "Media loaded." << std::endl;
            break;
        case QMediaPlayer::StalledMedia:
            std::cout << "Media stalled." << std::endl;
            break;
        case QMediaPlayer::BufferingMedia:
            std::cout << "Buffering media..." << std::endl;
            break;
        case QMediaPlayer::BufferedMedia:
            std::cout << "Media buffered." << std::endl;
            break;
        case QMediaPlayer::EndOfMedia:
            std::cout << "End of media reached." << std::endl;
            break;
        case QMediaPlayer::InvalidMedia:
            std::cout << "Invalid media." << std::endl;
            break;
        } });
    // Register queue events
    connect(queueWidget, &QueueWidget::itemAdded, this, [=](const QString &itemText)
            {
        std::cout << "Item added to queue: " << itemText.toStdString() << std::endl;
        // check player status
        qDebug() << "Player status:" << player->mediaStatus();
        if( player->mediaStatus() == QMediaPlayer::NoMedia || player->mediaStatus() == QMediaPlayer::EndOfMedia) {
            // then load media from the queue
            target->openFile(queueWidget->get());
            // target->mediaPlayer->play();
            // target->playVideo(playButton);
        } });
}

void ControlsWidget::closeEvent(QCloseEvent *event)
{
    std::cout << "ControlsWidget closed" << std::endl;
    target->close();
    event->accept(); // Accept the close event to allow the widget to close
}