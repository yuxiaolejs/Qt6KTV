#ifndef PLAYERWINDOW_HPP
#define PLAYERWINDOW_HPP

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

class PlayerWindow : public QWidget
{
public:
    PlayerWindow();
    void openFile(QString fileName);
    void switchAudio();
    void playVideo(QPushButton *playButton);
    void setVolume(float volume);
    void switchToEnd();
    QMediaPlayer *mediaPlayer;

private:
    QAudioOutput *audioOutput;
    QVideoWidget *videoWidget;
    int defaultAudioTrack = 0;
};

#endif