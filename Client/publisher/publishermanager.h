#ifndef PUBLISHERMANAGER_H
#define PUBLISHERMANAGER_H

#include <QVector>
#include <QString>
#include "Publisher.h"

// Thin wrapper around NetworkClient for publisher/book/catalog operations.
// Holds no local state itself anymore - the server is the single source of
// truth for the book catalog and publisher accounts.
class PublisherManager
{
public:
    PublisherManager();

    bool registerPublisher(const QString &username, const QString &password,
                           const QString &securityAnswer, QString &errorOut);

    bool changeUsername(const QString &newUsername, QString &errorOut);
    bool changePassword(const QString &oldPassword, const QString &newPassword, QString &errorOut);

    QVector<Book> getAllActiveBooks(QString &errorOut) const;
    QVector<Book> getBooksByGenre(genre g, QString &errorOut) const;
    QVector<Book> getBooksByPublisher(QString &errorOut) const;
    QVector<Book> getRecommendedBooks(QString &errorOut) const;
    QVector<Book> getNewestBooks(QString &errorOut) const;
    QVector<Book> getMostRatedBooks(QString &errorOut) const;
    QVector<Book> getFreeBooks(QString &errorOut) const;
    QVector<Book> getTopSellingBooks(QString &errorOut) const;
    QVector<Book> searchBooks(const QString &keyword, QString &errorOut) const;

    // coverFilePath/pdfFilePath are local filesystem paths chosen through a
    // file picker; pass empty strings to leave an existing file untouched
    // when editing.
    bool addBook(Book &book, const QString &coverFilePath, const QString &pdfFilePath, QString &errorOut);
    bool editBook(const Book &book, const QString &coverFilePath, const QString &pdfFilePath, QString &errorOut);
    bool applyDiscount(int bookId, double discount, QString &errorOut);
    bool setBookActive(int bookId, bool active, QString &errorOut);

    QVector<Publisher> getAllPublishers(QString &errorOut) const;
    bool setPublisherBlocked(int publisherId, bool blocked, QString &errorOut);
    bool deletePublisher(int publisherId, QString &errorOut);

    QVector<Book> getAllBooksAdmin(QString &errorOut) const;
    bool removeBookGlobally(int bookId, QString &errorOut);
    bool updateBookGlobally(const Book &book, QString &errorOut);
    bool deleteReviewAdmin(int bookId, int userId, QString &errorOut);
};

#endif // PUBLISHERMANAGER_H
