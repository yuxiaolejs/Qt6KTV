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

private:
    QMediaPlayer *mediaPlayer;
    void playVideo(QPushButton *playButton);
};

#endif