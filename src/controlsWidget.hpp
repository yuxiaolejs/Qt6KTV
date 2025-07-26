#ifndef CONTROLSWIDGET_HPP
#define CONTROLSWIDGET_HPP

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
#include <QCloseEvent>

#include "playerWindow.hpp"

class ControlsWidget : public QWidget
{
public:
    ControlsWidget(PlayerWindow *target);

private:
    PlayerWindow *target;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif