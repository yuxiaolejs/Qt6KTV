#ifndef MEDIAWIDGET_HPP
#define MEDIAWIDGET_HPP

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QStyle>
#include <QLabel>
#include <QHBoxLayout>
#include <QAudioOutput>
#include <QLineEdit>
#include <QListWidget>
#include <QStringList>
#include <vector>
#include <QDir>
#include "queueWidget.hpp"

class MediaWidget : public QWidget
{

public:
    MediaWidget(QWidget *parent = nullptr, QueueWidget *queueWidget = nullptr);

private slots:
    void filterList(const QString &text);

private:
    QueueWidget *queueWidget;
    QLineEdit *searchBar;
    QListWidget *listWidget;
    QStringList allItems;
    QString basePath = "/home/unics/Desktop/KTV";
    void readMediaFilesRecursively(const QString &path, const QString relativePath = QString());
};

#endif // MEDIAWIDGET_HPP