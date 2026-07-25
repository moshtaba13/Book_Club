#ifndef SHELF_H
#define SHELF_H

#include "Book.h"
#include <QString>
#include <QVector>

class Shelf
{
private:
    QString name;
    QVector<Book> books;
public:
    Shelf();
    Shelf(const QString &name);

    QString getName() const;
    void setName(const QString &newName);
    void addBook(const Book &book);
    void removeBook(int bookId);
    bool containsBook(int bookId) const;

    const QVector<Book>& getBooks() const;
    int bookCount() const;
    bool isEmpty() const;
};

#endif