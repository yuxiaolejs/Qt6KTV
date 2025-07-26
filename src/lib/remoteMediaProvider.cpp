#include "remoteMediaProvider.hpp"
#include <QDir>
#include <QFileInfoList>

RemoteMediaProvider::RemoteMediaProvider(QString basePath)
{
    this->basePath = basePath;
}
QStringList RemoteMediaProvider::listMedia()
{
    QStringList allItems;
    this->readMediaFilesRecursively(this->basePath, QString(), &allItems);
    return allItems;
}
QStringList RemoteMediaProvider::searchMedia(const QString &query)
{
    QStringList filteredItems;
    for (const QString &item : listMedia())
    {
        if (item.contains(query, Qt::CaseInsensitive))
        {
            filteredItems.append(item);
        }
    }
    return filteredItems;
}
QString RemoteMediaProvider::getLocalMediaPath(const QString &mediaName)
{
    return basePath + "/" + mediaName; // Assuming mediaName is a valid file name
}

void RemoteMediaProvider::readMediaFilesRecursively(const QString &path, QString relativePath, QStringList *allItems)
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