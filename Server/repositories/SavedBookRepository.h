#ifndef SAVEDBOOKREPOSITORY_H
#define SAVEDBOOKREPOSITORY_H

#include "../Database.h"
#include "../../Common/models/Book.h"
#include <QVector>

class SavedBookRepository
{
private:
    SavedBookRepository() = default;
    SavedBookRepository(const SavedBookRepository&) = delete;
    SavedBookRepository& operator=(const SavedBookRepository&) = delete;
public:
    static SavedBookRepository& instance();

    bool save(int userId, int bookId);
    bool remove(int userId, int bookId);
    bool hasBook(int userId, int bookId);
    QVector<Book> findByUser(int userId);
    QVector<int> findUsersByBook(int bookId);


};

#endif