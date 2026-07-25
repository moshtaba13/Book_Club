#ifndef SHELFREPOSITORY_H
#define SHELFREPOSITORY_H

#include <QVector>

#include "../Database.h"
#include "../../Common/models/Shelf.h"

class ShelfRepository
{
private:
    ShelfRepository() = default;
    ShelfRepository(const ShelfRepository&) = delete;
    ShelfRepository& operator=(const ShelfRepository&) = delete;
public:
    static ShelfRepository& instance();

    bool addShelf(int userId, const QString &name);
    bool removeShelf(int userId, const QString &name);
    bool renameShelf(int userId, const QString &oldName, const QString &newName);

    bool addBook(int userId, const QString &shelfName, int bookId);
    bool removeBook(int userId, const QString &shelfName, int bookId);
    bool moveBook(int userId, const QString &fromShelf, const QString &toShelf, int bookId);
    
    QVector<Shelf> findByUser(int userId);
    bool shelfExists(int userId, const QString &name);

};

#endif