#ifndef QUEUEWIDGET_HPP
#define QUEUEWIDGET_HPP

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

class QueueWidget : public QWidget
{
    Q_OBJECT
public:
    QueueWidget(QWidget *parent = nullptr);
    void put(const QString &itemText);
    QString get();
    QString get(int index) const;
    int indexOf(const QString &itemText) const;

private slots:
    void renderList();

private:
    QListWidget *listWidget;
    QStringList allItems;
signals:
    void itemAdded(const QString &itemText);
    void itemRemoved(const QString &itemText);
    void itemMovedToTop(const QString &itemText);
};

#endif // QUEUEWIDGET_HPP