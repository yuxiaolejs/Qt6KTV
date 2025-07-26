#ifndef LOCALMEDIAPROVIDER_HPP
#define LOCALMEDIAPROVIDER_HPP
#include <QString>

class LocalMediaProvider
{
public:
    LocalMediaProvider(QString basePath);
    QStringList listMedia();
    QString getLocalMediaPath(const QString &mediaName);

private:
    QString basePath; // Base path for media files
    void readMediaFilesRecursively(const QString &path, QString relativePath, QStringList *allItems);
};
#endif