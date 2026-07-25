#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include <QString>
#include <QVector>

#include "../repositories/BookRepository.h"
#include "../repositories/SavedBookRepository.h"
#include "../repositories/MemberRepository.h"
#include "../../Common/models/Book.h"

using namespace std;

class BookManager
{
private:
    BookManager() = default;
    BookManager(const BookManager&) = delete;
    BookManager& operator=(const BookManager&) = delete;

    void notifUsersNewBook(const Book &book);
    void notifUsersDiscount(const Book &book);
public:
    static BookManager& instance();

    bool addBook(int publisherId, const QString &publisherUsername, Book &book);
    bool applyDiscount(int bookId, double discount);

    bool addReview(int bookId, Review &review);

    bool activeBook(int bookId);
    bool inactiveBook(int bookId);

    QVector<Book> getTopSellingBooks(int limit = 5);
    QVector<Book> getLeastSellingBooks(int limit = 5);
    QVector<Book> getNewestBooks(int limit = 10);
    QVector<Book> getMostRatedBooks(int limit = 10);
    QVector<Book> getFreeBooks();
    QVector<Book> getRecommendedBooks(const QVector<genre> &genres);

};

#endif