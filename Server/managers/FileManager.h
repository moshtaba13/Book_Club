#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QByteArray>

class FileManager
{
private:
    FileManager() = default;
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

    bool ensureDirectoriesExist();

    const QString pdfDir   = "data/pdfs/";
    const QString coverDir = "data/covers/";

public:
    static FileManager& instance();

    QString savePdf(int bookId, const QByteArray &data);
    QString saveCover(int bookId, const QByteArray &data, const QString &extension);

    bool deletePdf(const QString &filePath);
    bool deleteCover(const QString &filePath);

    QString pdfPath(int bookId) const;
    QString coverPath(int bookId, const QString &extension) const;

};

#endif