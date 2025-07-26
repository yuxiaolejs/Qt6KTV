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
#include <QTimer>
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
    audioOutput->setVolume(0.5f); // Set initial volume

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);

    layout->setContentsMargins(0, 0, 0, 0);
    this->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    qDebug() << "Video output:" << mediaPlayer->videoOutput();
    // mediaPlayer->connect(mediaPlayer, &QMediaPlayer::activeTracksChanged, this, [this]()
                        //  { std::cout << "tracks changed" << std::endl; });
    connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [](QMediaPlayer::PlaybackState state)
            { qDebug() << "playbackStateChanged:" << state; });
}

void PlayerWindow::openFile(QString fileName)
{
    // QString fileName = QFileDialog::getOpenFileName(nullptr, "Open Video File", "", "Video Files (*.mp4 *.avi *.mkv)");
    qDebug() << "Opening file: " << fileName;
    if (!fileName.isEmpty())
    {
        mediaPlayer->setSource(QUrl::fromLocalFile(fileName));
        mediaPlayer->play();
    }
}

void PlayerWindow::switchAudio()
{
    // std::cout << "Pausing for audio switch" << std::endl;
    mediaPlayer->pause();
    int track = mediaPlayer->activeAudioTrack();
    auto ad = mediaPlayer->audioTracks();
    if(ad.size() < 2)
    {
        qDebug() << "Not enough audio tracks to switch";
        return; // Not enough audio tracks to switch
    }
    if (track == 1)
    {
        mediaPlayer->setActiveAudioTrack(0);
    }
    else
    {
        mediaPlayer->setActiveAudioTrack(1);
    }
    for (int i = 0; i < ad.size(); i++)
    {
        std::cout << "Audio track: " << ad[i].value(QMediaMetaData::AudioBitRate).toInt() << std::endl;
    }
    mediaPlayer->play();
    // std::cout << "Playing after audio switch" << std::endl;
}

void PlayerWindow::playVideo(QPushButton *playButton)
{
    qDebug() << "PAUSE/PLAY button clicked, current state: " << mediaPlayer->playbackState() << ", media status: " << mediaPlayer->mediaStatus();
    qDebug() << "Video output:" << mediaPlayer->videoOutput();
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
    // std::cout << "Volume: " << volume << std::endl;
    audioOutput->setVolume(volume);
}

void PlayerWindow::switchToEnd()
{
    if (mediaPlayer->mediaStatus() == QMediaPlayer::LoadedMedia ||
        mediaPlayer->mediaStatus() == QMediaPlayer::BufferedMedia)
    {
        qint64 duration = mediaPlayer->duration();
        if (duration > 0)
            mediaPlayer->setPosition(duration);
    }
}