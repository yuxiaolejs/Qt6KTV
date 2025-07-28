#ifndef REMOTEMEDIAPROVIDER_HPP
#define REMOTEMEDIAPROVIDER_HPP
#include <QString>
#include <QNetworkAccessManager>

class RemoteMediaProvider : public QObject
{
    Q_OBJECT
public:
    RemoteMediaProvider(QString basePath);
    ~RemoteMediaProvider();
    QStringList listMedia();
    QStringList searchMedia(const QString &query);
    QString getLocalMediaPath(const QString &mediaName);

private:
    QString basePath; // Base path for media files
    QNetworkAccessManager* manager;
signals:
    void mediaListUpdated(QStringList mediaList);
    void localMediaPathReady(QString localPath); 
};
#endif