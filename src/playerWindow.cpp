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
#include <QMediaMetaData>
#include <iostream>

#include "playerWindow.hpp"

PlayerWindow::PlayerWindow() : QWidget()
{
    this->mediaPlayer = new QMediaPlayer(this);

    // Create video widget
    videoWidget = new QVideoWidget(this);

    audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setVideoOutput(videoWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);

    layout->setContentsMargins(0, 0, 0, 0);
    this->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    mediaPlayer->connect(mediaPlayer, &QMediaPlayer::activeTracksChanged, this, [this]()
                         { std::cout << "111111111111111111111111111111Active tracks changed" << std::endl; });
}

void PlayerWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open Video File", "", "Video Files (*.mp4 *.avi *.mkv)");
    if (!fileName.isEmpty())
    {
        mediaPlayer->setSource(QUrl::fromLocalFile(fileName));
        mediaPlayer->play();
    }
}

void PlayerWindow::switchAudio()
{
    std::cout << "Pausing for audio switch" << std::endl;
    mediaPlayer->pause();
    int track = mediaPlayer->activeAudioTrack();
    if (track == 1)
    {
        mediaPlayer->setActiveAudioTrack(0);
    }
    else
    {
        mediaPlayer->setActiveAudioTrack(1);
    }
    auto ad = mediaPlayer->audioTracks();
    for (int i = 0; i < ad.size(); i++)
    {
        std::cout << "Audio track: " << ad[i].value(QMediaMetaData::AudioBitRate).toInt() << std::endl;
    }
    mediaPlayer->play();
    std::cout << "Playing after audio switch" << std::endl;
}

void PlayerWindow::playVideo(QPushButton *playButton)
{
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState)
    {
        mediaPlayer->pause();
        playButton->setIcon(playButton->style()->standardIcon(QStyle::SP_MediaPlay));
    }
    else
    {
        mediaPlayer->play();
        playButton->setIcon(playButton->style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void PlayerWindow::setVolume(float volume)
{
    std::cout << "Volume: " << volume << std::endl;
    audioOutput->setVolume(volume);
}