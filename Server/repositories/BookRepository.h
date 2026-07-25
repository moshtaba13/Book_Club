#ifndef BOOKREPOSITORY_H
#define BOOKREPOSITORY_H

#include <QVector>

#include "../Database.h"
#include "../../Common/models/Book.h"
#include "../../Common/protocol.h"

class BookRepository
{
private:
    BookRepository() = default;
    BookRepository(const BookRepository&) = delete;
    BookRepository& operator=(const BookRepository&) = delete;

    Book mapToBook(QSqlQuery &query);
    Review mapToReview(QSqlQuery &query);
public:
    static BookRepository& instance();

    bool save(int publisherId, Book &book);

    Book findById(int id);
    QVector<Book> findAll();
    QVector<Book> findAllActive();
    QVector<Book> findByPublisher(int publisherId);
    QVector<Book> findByGenre(genre g);
    QVector<Book> search(const QString &keyword);

    bool update(const Book &book);
    bool updateDiscount(int bookId, double discount);
    bool updateActiveStatus(int bookId, bool isActive);
    bool updateSalesCount(int bookId, int count);
    bool updateAverageRating(int bookId, double rating);

    bool remove(int bookId);

    bool saveReview(int bookId, const Review &review);
    bool updateReview(int bookId, const Review &review);
    bool removeReview(int bookId, int userId);
    QVector<Review> findReviews(int bookId);

};

#endif