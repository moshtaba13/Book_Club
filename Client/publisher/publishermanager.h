#ifndef PUBLISHERMANAGER_H
#define PUBLISHERMANAGER_H

#include <QVector>
#include <QString>
#include "Publisher.h"

class PublisherManager
{
private:
    QVector<Publisher> registeredPublishers;

public:
    PublisherManager();

    bool registerPublisher(const Publisher &newPublisher);
    bool usernameExists(const QString &username) const;
    bool authenticate(const QString &username, const QString &password, Publisher &foundPublisher);
    bool updatePublisher(const Publisher &updatedPublisher);
    bool getSecurityAnswer(const QString &username, QString &answerOut);
    bool resetPassword(const QString &username, const QString &securityAnswer, const QString &newPassword);
    bool isUsernameTakenByAnother(const QString &username, int excludeId) const;
    QVector<Book> getAllActiveBooks() const;
    QVector<Publisher> getAllPublishers() const;
    bool setPublisherBlocked(int publisherId, bool blocked);
    bool deletePublisher(int publisherId);

    QVector<Book> getAllBooks() const;
    bool removeBookGlobally(int bookId);
    bool updateBookGlobally(const Book &book);
};

#endif // PUBLISHERMANAGER_H