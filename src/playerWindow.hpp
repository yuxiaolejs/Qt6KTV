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
    void openFile();
    void switchAudio();
    void playVideo(QPushButton *playButton);
    void setVolume(float volume);

private:
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    QVideoWidget *videoWidget;
};

#endif