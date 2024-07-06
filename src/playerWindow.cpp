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

#include "playerWindow.hpp"

PlayerWindow::PlayerWindow() : QWidget()
{
    this->mediaPlayer = new QMediaPlayer(this);

    // Create video widget
    QVideoWidget *videoWidget = new QVideoWidget(this);

    auto audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setVideoOutput(videoWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);

    layout->setContentsMargins(0, 0, 0, 0);
    this->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
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
    int track = mediaPlayer->activeAudioTrack();
    if (track == 1)
    {
        mediaPlayer->setActiveAudioTrack(0);
    }
    else
    {
        mediaPlayer->setActiveAudioTrack(1);
    }
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