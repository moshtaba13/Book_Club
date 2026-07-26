#include "FileManager.h"
#include <QFile>
#include <QDir>
#include <QDebug>

FileManager& FileManager::instance() {
    static FileManager instance;
    return instance;
}

bool FileManager::ensureDirectoriesExist()
{
    QDir dir;
    if (!dir.exists(pdfDir))
        dir.mkpath(pdfDir);
    if (!dir.exists(coverDir))
        dir.mkpath(coverDir);
    return true;
}

QString FileManager::pdfPath(int bookId) const{
    return pdfDir + "book_" + QString::number(bookId) + ".pdf";
}

QString FileManager::coverPath(int bookId, const QString &extension) const {
    return coverDir + "book_" + QString::number(bookId) + "." + extension;
}

QString FileManager::saveCover(int bookId, const QByteArray &data, const QString &extension)
{
    ensureDirectoriesExist();

    QString path = coverPath(bookId, extension);
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "FileManager::saveCover error:" << file.errorString();
        return QString();
    }

    file.write(data);
    file.close();

    qDebug() << "Cover saved:" << path;
    return path;
}

QString FileManager::savePdf(int bookId, const QByteArray &data)
{
    ensureDirectoriesExist();

    QString path = pdfPath(bookId);
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "FileManager::savePdf error:" << file.errorString();
        return QString();
    }

    file.write(data);
    file.close();

    qDebug() << "PDF saved:" << path;
    return path;
}

bool FileManager::deletePdf(const QString &filePath)
{
    if (filePath.isEmpty())
        return true;

    QFile file(filePath);
    if (!file.exists())
        return true;

    return file.remove();
}

bool FileManager::deleteCover(const QString &filePath)
{
    if (filePath.isEmpty())
        return true;

    QFile file(filePath);
    if (!file.exists())
        return true;

    return file.remove();
}
