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
#include "lib/localMediaProvider.hpp"
#include "lib/remoteMediaProvider.hpp"

class MediaWidget : public QWidget
{

public:
    MediaWidget(QWidget *parent = nullptr, QueueWidget *queueWidget = nullptr);

private slots:
    void filterList(const QString &text);

private:
    RemoteMediaProvider *mediaProvider;
    QueueWidget *queueWidget;
    QLineEdit *searchBar;
    QListWidget *listWidget;
    QStringList allItems;
    QString basePath = "http://127.0.0.1:18010";
};

#endif // MEDIAWIDGET_HPP