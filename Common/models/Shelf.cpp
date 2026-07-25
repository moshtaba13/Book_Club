#include "Shelf.h"

Shelf::Shelf() {}

Shelf::Shelf(const QString &name)
    : name(name) {}

QString Shelf::getName() const { return name;}
void Shelf::setName(const QString &newName) { name = newName; }

void Shelf::addBook(const Book &book) {
    if (!containsBook(book.getId()))
        books.append(book);
}

void Shelf::removeBook(int bookId) {
    for (int i = 0; i < books.size(); ++i) {
        if (books[i].getId() == bookId) {
            books.removeAt(i);
            break;
        }
    }
}

bool Shelf::containsBook(int bookId) const {
    for (const Book &b : books)
        if (b.getId() == bookId)
            return true;
    return false;
}

const QVector<Book>& Shelf::getBooks() const{ return books;}

int Shelf::bookCount() const{ return books.size();}

bool Shelf::isEmpty() const{ return books.isEmpty();}