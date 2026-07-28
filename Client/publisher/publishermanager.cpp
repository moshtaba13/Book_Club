#include "publishermanager.h"

#include <QJsonArray>
#include <QFile>
#include <QFileInfo>

#include "networkclient.h"
#include "modelserializer.h"
#include "protocol.h"

namespace {

QString readFileAsBase64(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return QString();
    return QString::fromLatin1(file.readAll().toBase64());
}

}

PublisherManager::PublisherManager() {}

bool PublisherManager::registerPublisher(const QString &username, const QString &password,
                                         const QString &securityAnswer, QString &errorOut)
{
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    data["security_answer"] = securityAnswer;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::RegisterPublisher, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Registration failed.");
        return false;
    }
    return true;
}

bool PublisherManager::changeUsername(const QString &newUsername, QString &errorOut)
{
    QJsonObject data;
    data["username"] = newUsername;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::ChangeUsername, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not change the username.");
        return false;
    }
    return true;
}

bool PublisherManager::changePassword(const QString &oldPassword, const QString &newPassword, QString &errorOut)
{
    QJsonObject data;
    data["old_password"] = oldPassword;
    data["new_password"] = newPassword;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::ChangePassword, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not change the password.");
        return false;
    }
    return true;
}

QVector<Book> PublisherManager::getAllActiveBooks(QString &errorOut) const
{
    QVector<Book> books;
    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetAllBooks);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not load the book catalog.");
        return books;
    }

    for (const QJsonValue &v : response.value("data").toObject().value("books").toArray())
        books.append(ModelSerializer::bookFromJson(v.toObject()));

    return books;
}

QVector<Book> PublisherManager::getBooksByGenre(genre g, QString &errorOut) const
{
    QVector<Book> books;
    QJsonObject data;
    data["genre"] = genreToString(g);

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetBooksByGenre, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not load books for this genre.");
        return books;
    }

    for (const QJsonValue &v : response.value("data").toObject().value("books").toArray())
        books.append(ModelSerializer::bookFromJson(v.toObject()));

    return books;
}

QVector<Book> PublisherManager::getBooksByPublisher(QString &errorOut) const
{
    QVector<Book> books;
    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetBooksByPublisher);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not load your published books.");
        return books;
    }

    for (const QJsonValue &v : response.value("data").toObject().value("books").toArray())
        books.append(ModelSerializer::bookFromJson(v.toObject()));

    return books;
}

namespace {

QVector<Book> booksFromListRequest(RequestType type, const QJsonObject &data, QString &errorOut, const QString &fallbackError)
{
    QVector<Book> books;
    QJsonObject response = NetworkClient::instance().sendRequest(type, data);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString(fallbackError);
        return books;
    }

    for (const QJsonValue &v : response.value("data").toObject().value("books").toArray())
        books.append(ModelSerializer::bookFromJson(v.toObject()));

    return books;
}

}

QVector<Book> PublisherManager::getRecommendedBooks(QString &errorOut) const
{
    return booksFromListRequest(RequestType::GetRecommendedBooks, {}, errorOut, "Could not load recommended books.");
}

QVector<Book> PublisherManager::getNewestBooks(QString &errorOut) const
{
    return booksFromListRequest(RequestType::GetNewestBooks, {}, errorOut, "Could not load newest books.");
}

QVector<Book> PublisherManager::getMostRatedBooks(QString &errorOut) const
{
    return booksFromListRequest(RequestType::GetMostRatedBooks, {}, errorOut, "Could not load top rated books.");
}

QVector<Book> PublisherManager::getFreeBooks(QString &errorOut) const
{
    return booksFromListRequest(RequestType::GetFreeBooks, {}, errorOut, "Could not load free books.");
}

QVector<Book> PublisherManager::getTopSellingBooks(QString &errorOut) const
{
    return booksFromListRequest(RequestType::GetTopSellingBooks, {}, errorOut, "Could not load best sellers.");
}

QVector<Book> PublisherManager::searchBooks(const QString &keyword, QString &errorOut) const
{
    QJsonObject data;
    data["keyword"] = keyword;
    return booksFromListRequest(RequestType::SearchBooks, data, errorOut, "Search failed.");
}

bool PublisherManager::addBook(Book &book, const QString &coverFilePath, const QString &pdfFilePath, QString &errorOut)
{
    QJsonObject data;
    data["title"] = book.getTitle();
    data["author"] = book.getAuthor();
    data["genre"] = genreToString(book.getGenre());
    data["description"] = book.getDiscription();
    data["price"] = book.getPrice();
    data["discount"] = book.getDiscount();
    data["cover_name"] = QFileInfo(coverFilePath).fileName();
    data["cover_data"] = readFileAsBase64(coverFilePath);
    data["pdf_data"] = readFileAsBase64(pdfFilePath);

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::AddBook, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not add the book.");
        return false;
    }

    book = ModelSerializer::bookFromJson(response.value("data").toObject());
    return true;
}

bool PublisherManager::editBook(const Book &book, const QString &coverFilePath, const QString &pdfFilePath, QString &errorOut)
{
    QJsonObject data;
    data["id"] = book.getId();
    data["title"] = book.getTitle();
    data["author"] = book.getAuthor();
    data["genre"] = genreToString(book.getGenre());
    data["description"] = book.getDiscription();
    data["price"] = book.getPrice();
    data["discount"] = book.getDiscount();

    if (QFileInfo(coverFilePath).isAbsolute()) {
        data["cover_name"] = QFileInfo(coverFilePath).fileName();
        data["cover_data"] = readFileAsBase64(coverFilePath);
    }
    if (QFileInfo(pdfFilePath).isAbsolute())
        data["pdf_data"] = readFileAsBase64(pdfFilePath);

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::EditBook, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not update the book.");
        return false;
    }
    return true;
}

bool PublisherManager::applyDiscount(int bookId, double discount, QString &errorOut)
{
    QJsonObject data;
    data["book_id"] = bookId;
    data["discount"] = discount;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::ApplyDiscount, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not apply the discount.");
        return false;
    }
    return true;
}

bool PublisherManager::setBookActive(int bookId, bool active, QString &errorOut)
{
    QJsonObject data;
    data["book_id"] = bookId;

    QJsonObject response = NetworkClient::instance().sendRequest(
        active ? RequestType::RestoreBook : RequestType::InActiveBook, data);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not update the book's status.");
        return false;
    }
    return true;
}

QVector<Publisher> PublisherManager::getAllPublishers(QString &errorOut) const
{
    QVector<Publisher> publishers;
    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetAllPublishers);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not load publishers.");
        return publishers;
    }

    for (const QJsonValue &v : response.value("data").toObject().value("publishers").toArray())
        publishers.append(ModelSerializer::publisherFromJson(v.toObject()));

    return publishers;
}

bool PublisherManager::setPublisherBlocked(int publisherId, bool blocked, QString &errorOut)
{
    QJsonObject data;
    data["user_id"] = publisherId;

    QJsonObject response = NetworkClient::instance().sendRequest(
        blocked ? RequestType::BlockUser : RequestType::UnblockUser, data);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not update the publisher's status.");
        return false;
    }
    return true;
}

bool PublisherManager::deletePublisher(int publisherId, QString &errorOut)
{
    QJsonObject data;
    data["user_id"] = publisherId;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::DeleteUser, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not delete the publisher.");
        return false;
    }
    return true;
}

QVector<Book> PublisherManager::getAllBooksAdmin(QString &errorOut) const
{
    QVector<Book> books;
    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetAllBooksAdmin);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not load books.");
        return books;
    }

    for (const QJsonValue &v : response.value("data").toObject().value("books").toArray())
        books.append(ModelSerializer::bookFromJson(v.toObject()));

    return books;
}

bool PublisherManager::removeBookGlobally(int bookId, QString &errorOut)
{
    QJsonObject data;
    data["book_id"] = bookId;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::DeleteBookAdmin, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not delete the book.");
        return false;
    }
    return true;
}

bool PublisherManager::updateBookGlobally(const Book &book, QString &errorOut)
{
    QJsonObject data;
    data["id"] = book.getId();
    data["title"] = book.getTitle();
    data["author"] = book.getAuthor();
    data["genre"] = genreToString(book.getGenre());
    data["description"] = book.getDiscription();
    data["price"] = book.getPrice();
    data["discount"] = book.getDiscount();

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::EditBookAdmin, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not update the book.");
        return false;
    }
    return true;
}

bool PublisherManager::deleteReviewAdmin(int bookId, int userId, QString &errorOut)
{
    QJsonObject data;
    data["book_id"] = bookId;
    data["user_id"] = userId;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::DeleteReviewAdmin, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not delete the review.");
        return false;
    }
    return true;
}
