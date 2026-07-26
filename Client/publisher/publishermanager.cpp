#include "PublisherManager.h"

PublisherManager::PublisherManager() {}

bool PublisherManager::usernameExists(const QString &username) const
{
    for (const Publisher &p : registeredPublishers) {
        if (p.getUsername() == username) {
            return true;
        }
    }
    return false;
}

bool PublisherManager::registerPublisher(const Publisher &newPublisher)
{
    if (usernameExists(newPublisher.getUsername())) {
        return false;
    }
    registeredPublishers.append(newPublisher);
    return true;
}

bool PublisherManager::authenticate(const QString &username, const QString &password, Publisher &foundPublisher)
{
    for (const Publisher &p : registeredPublishers) {
        if (p.getUsername() == username && p.getPassword() == password) {
            foundPublisher = p;
            return true;
        }
    }
    return false;
}

bool PublisherManager::updatePublisher(const Publisher &updatedPublisher)
{
    for (int i = 0; i < registeredPublishers.size(); ++i) {
        if (registeredPublishers[i].getId() == updatedPublisher.getId()) {
            registeredPublishers[i] = updatedPublisher;
            return true;
        }
    }
    return false;
}

bool PublisherManager::getSecurityAnswer(const QString &username, QString &answerOut)
{
    for (const Publisher &p : registeredPublishers) {
        if (p.getUsername() == username) {
            answerOut = p.getSecurityAnswer();
            return true;
        }
    }
    return false;
}

bool PublisherManager::resetPassword(const QString &username, const QString &securityAnswer, const QString &newPassword)
{
    for (int i = 0; i < registeredPublishers.size(); ++i) {
        if (registeredPublishers[i].getUsername() == username &&
            registeredPublishers[i].getSecurityAnswer() == securityAnswer) {
            registeredPublishers[i].setPassword(newPassword);
            return true;
        }
    }
    return false;
}

bool PublisherManager::isUsernameTakenByAnother(const QString &username, int excludeId) const
{
    for (const Publisher &p : registeredPublishers) {
        if (p.getUsername() == username && p.getId() != excludeId) {
            return true;
        }
    }
    return false;
}
QVector<Book> PublisherManager::getAllActiveBooks() const
{
    QVector<Book> result;
    for (const Publisher &p : registeredPublishers) {
        for (const Book &b : p.getPublishedBooks()) {
            if (b.getisActive()) {
                result.append(b);
            }
        }
    }
    return result;
}
QVector<Publisher> PublisherManager::getAllPublishers() const
{
    return registeredPublishers;
}

bool PublisherManager::setPublisherBlocked(int publisherId, bool blocked)
{
    for (int i = 0; i < registeredPublishers.size(); ++i) {
        if (registeredPublishers[i].getId() == publisherId) {
            registeredPublishers[i].setBlocked(blocked);
            return true;
        }
    }
    return false;
}

bool PublisherManager::deletePublisher(int publisherId)
{
    for (int i = 0; i < registeredPublishers.size(); ++i) {
        if (registeredPublishers[i].getId() == publisherId) {
            registeredPublishers.removeAt(i);
            return true;
        }
    }
    return false;
}
QVector<Book> PublisherManager::getAllBooks() const
{
    QVector<Book> result;
    for (const Publisher &p : registeredPublishers) {
        for (const Book &b : p.getPublishedBooks()) {
            result.append(b);
        }
    }
    return result;
}

bool PublisherManager::removeBookGlobally(int bookId)
{
    for (int i = 0; i < registeredPublishers.size(); ++i) {
        for (const Book &b : registeredPublishers[i].getPublishedBooks()) {
            if (b.getId() == bookId) {
                registeredPublishers[i].removePublishedBook(bookId);
                return true;
            }
        }
    }
    return false;
}

bool PublisherManager::updateBookGlobally(const Book &book)
{
    for (int i = 0; i < registeredPublishers.size(); ++i) {
        for (const Book &b : registeredPublishers[i].getPublishedBooks()) {
            if (b.getId() == book.getId()) {
                registeredPublishers[i].updatePublishedBook(book);
                return true;
            }
        }
    }
    return false;
}