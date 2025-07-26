#include "localMediaProvider.hpp"
#include <QDir>
#include <QFileInfoList>

LocalMediaProvider::LocalMediaProvider(QString basePath)
{
    this->basePath = basePath;
}
QStringList LocalMediaProvider::listMedia()
{
    QStringList allItems;
    this->readMediaFilesRecursively(this->basePath, QString(), &allItems);
    return allItems;
}
QString LocalMediaProvider::getLocalMediaPath(const QString &mediaName)
{
    return basePath + "/" + mediaName; // Assuming mediaName is a valid file name
}

void LocalMediaProvider::readMediaFilesRecursively(const QString &path, QString relativePath, QStringList *allItems)
{
    QDir dir(path);
    if (!dir.exists())
        return ;

    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QFileInfo &fileInfo : entries)
    {
        if (fileInfo.isFile())
        {
            allItems->append(relativePath + fileInfo.fileName());
        }
    }

    QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &subDir : subDirs)
    {
        readMediaFilesRecursively(subDir.absoluteFilePath(), relativePath + subDir.fileName() + "/", allItems);
    }
    return ;
}