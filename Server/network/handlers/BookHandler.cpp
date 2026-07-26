#include "BookHandler.h"
#include "../ClientHandler.h"
#include "../../managers/BookManager.h"
#include "../../managers/PurchaseManager.h"
#include "../../managers/FileManager.h"
#include "../../repositories/BookRepository.h"

#include <QFile>

QJsonObject BookHandler::handleGetAllBooks()
{
    QVector<Book> books = BookRepository::instance().findAllActive();
    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetBooksByGenre(const QJsonObject &data)
{
    genre g = stringToGenre(data["genre"].toString());
    QVector<Book> books = BookRepository::instance().findByGenre(g);

    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetBooksByPublisher(const QJsonObject &data, ClientHandler *client)
{
    if (!isLoggedIn(client))
        return unauthorized();

    QVector<Book> books = BookRepository::instance().findByPublisher(client->currentUser()->getId());

    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleSearchBooks(const QJsonObject &data)
{
    QString keyword = data["keyword"].toString();
    QVector<Book> books = BookRepository::instance().search(keyword);

    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetBookDetails(const QJsonObject &data)
{
    int bookId = data["book_id"].toInt();
    Book book = BookRepository::instance().findById(bookId);

    if (book.getId() == 0)
        return notFound("book not founded");

    return success(bookToJsonFull(book));
}

QJsonObject BookHandler::handleAddBook(const QJsonObject &data, ClientHandler *client)
{
    if (!isPublisher(client))
        return unauthorized();

    Book book;
    book.setTitle(data["title"].toString());
    book.setAuthor(data["author"].toString());
    book.setGenre(stringToGenre(data["genre"].toString()));
    book.setDescription(data["description"].toString());
    book.setPrice(data["price"].toDouble());
    book.setDiscount(data["discount"].toDouble());
    

    if (!BookManager::instance().addBook(client->currentUser()->getId(), client->currentUser()->getUsername(),book))
        return failure("Error adding book");
    

    QByteArray pdfData = QByteArray::fromBase64(data["pdf_data"].toString().toLatin1());
    QString pdfPath = FileManager::instance().savePdf(book.getId(), pdfData);

    if (pdfPath.isEmpty())
        return failure("Error saving PDF");

    book.setPdfPath(pdfPath);
    BookRepository::instance().update(book);

    QString coverName = data["cover_name"].toString();
    QString ext = coverName.split(".").last();

    QByteArray coverData = QByteArray::fromBase64(data["cover_data"].toString().toLatin1());
    QString coverPath = FileManager::instance().saveCover(book.getId(), coverData, ext);

    if (coverPath.isEmpty())
        return failure("Error saving cover");

    book.setImagePath(coverPath);
    BookRepository::instance().update(book);

    return success(bookToJsonFull(book));
}

QJsonObject BookHandler::handleEditBook(const QJsonObject &data, ClientHandler *client)
{
    if (!isPublisher(client))
        return unauthorized();

    Book book = BookRepository::instance().findById(data["id"].toInt());

    if (book.getId() == 0)
        return notFound("book not founed");

    book.setTitle(data["title"].toString());
    book.setAuthor(data["author"].toString());
    book.setGenre(stringToGenre(data["genre"].toString()));
    book.setDescription(data["description"].toString());
    book.setPrice(data["price"].toDouble());
    book.setDiscount(data["discount"].toDouble());

    if (data.contains("cover_data") && !data["cover_data"].toString().isEmpty()) {
        QString coverName = data["cover_name"].toString();
        QString ext = coverName.split(".").last();

        QByteArray coverData = QByteArray::fromBase64(data["cover_data"].toString().toLatin1());
        QString coverPath = FileManager::instance().saveCover(book.getId(), coverData, ext);

        if (coverPath.isEmpty())
            return failure("Error updating book cover");

        book.setImagePath(coverPath);
    }

    if (data.contains("pdf_data") && !data["pdf_data"].toString().isEmpty()) {
        QByteArray pdfData = QByteArray::fromBase64(data["pdf_data"].toString().toLatin1());
        QString pdfPath = FileManager::instance().savePdf(book.getId(), pdfData);

        if (pdfPath.isEmpty())
            return failure("Error updating book pdf");

        book.setPdfPath(pdfPath);
    }

    if (!BookRepository::instance().update(book))
        return failure("Error in book editing");

    return success();
}

QJsonObject BookHandler::handleInActiveBook(const QJsonObject &data, ClientHandler *client) 
{
    if (!isPublisher(client))
        return unauthorized();

    if (!BookManager::instance().inactiveBook(data["book_id"].toInt()))
        return failure("Error inActive book");

    return success();
}

QJsonObject BookHandler::handleRestoreBook(const QJsonObject &data, ClientHandler *client)
{
    if (!isPublisher(client))
        return unauthorized();

    if (!BookManager::instance().activeBook(data["book_id"].toInt()))
        return failure("Error returning book");

    return success();
}

QJsonObject BookHandler::handleApplyDiscount(const QJsonObject &data, ClientHandler *client)
{
    if (!isPublisher(client))
        return unauthorized();

    if (!BookManager::instance().applyDiscount(data["book_id"].toInt(), data["discount"].toDouble()))
        return failure("Error in applying discount");

    return success();
}

QJsonObject BookHandler::handleAddReview(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();

    if (!PurchaseRepository::instance().hasPurchased(client->currentUser()->getId(), bookId))
        return failure("You must purchase the book first.");

    Review review(
        client->currentUser()->getId(),
        data["stars"].toInt(),
        data["comment"].toString()
    );

    if (!BookManager::instance().addReview(bookId, review))
        return failure("Error in review registration");

    return success();
}

QJsonObject BookHandler::handleEditReview(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();
    Review review(
        client->currentUser()->getId(),
        data["stars"].toInt(),
        data["comment"].toString()
    );
    review.Edit(data["stars"].toInt(), data["comment"].toString());

    if (!BookRepository::instance().updateReview(bookId, review))
        return failure("Error editing review");

    return success();
}

QJsonObject BookHandler::handleDeleteReview(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    if (!BookRepository::instance().removeReview(
            data["book_id"].toInt(),
            client->currentUser()->getId()))
        return failure("Error deleting review");

    return success();
}

QJsonObject BookHandler::handleGetReviews(const QJsonObject &data)
{
    int bookId = data["book_id"].toInt();
    QVector<Review> reviews = BookRepository::instance().findReviews(bookId);

    QJsonArray arr;
    for (const Review &r : reviews)
        arr.append(reviewToJson(r));

    QJsonObject result;
    result["reviews"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetTopSellingBooks()
{
    QVector<Book> books = BookManager::instance().getTopSellingBooks();
    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetLeastSellingBooks()
{
    QVector<Book> books = BookManager::instance().getLeastSellingBooks();
    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetNewestBooks()
{
    QVector<Book> books = BookManager::instance().getNewestBooks();
    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetMostRatedBooks()
{
    QVector<Book> books = BookManager::instance().getMostRatedBooks();
    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetFreeBooks()
{
    QVector<Book> books = BookManager::instance().getFreeBooks();
    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetRecommendedBooks(ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    User *user = dynamic_cast<User*>(client->currentUser().get());

    QVector<Book> books = BookManager::instance().getRecommendedBooks(user->getfavoriteGenres());
    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    return success(result);
}

QJsonObject BookHandler::handleGetBookFile(const QJsonObject &data, ClientHandler *client)
{
    if (!isLoggedIn(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();

    if (!PurchaseRepository::instance().hasPurchased(client->currentUser()->getId(), bookId))
        return failure("you must buy this book first");

    Book book = BookRepository::instance().findById(bookId);

    QFile file(book.getPdfPath());
    if (!file.open(QIODevice::ReadOnly))
        return notFound("pdf not founded");

    QByteArray fileData = file.readAll();
    file.close();

    QJsonObject result;
    result["pdf_data"] = QString::fromLatin1(fileData.toBase64());
    result["book_id"] = bookId;

    return success(result);
}

QJsonObject BookHandler::handleGetBookCover(const QJsonObject &data, ClientHandler *client)
{
    if (!isLoggedIn(client))
        return unauthorized();
    
    int bookId = data["book_id"].toInt();
    Book book = BookRepository::instance().findById(bookId);

    if (book.getId() == 0)
        return notFound("book not founded");

    QFile file(book.getImagePath());
    if (!file.open(QIODevice::ReadOnly))
        return notFound("cover not founded");

    QByteArray coverData = file.readAll();
    file.close();

    QJsonObject result;
    result["cover_data"] = QString::fromLatin1(coverData.toBase64());
    result["book_id"] = bookId;

    return success(result);
}

QJsonObject BookHandler::handle(RequestType type, const QJsonObject &data, ClientHandler *client)
{
    switch (type) {
    case RequestType::GetAllBooks:
        return handleGetAllBooks();
    case RequestType::GetBooksByGenre:
        return handleGetBooksByGenre(data);
    case RequestType::GetBooksByPublisher:
        return handleGetBooksByPublisher(data, client);
    case RequestType::SearchBooks:
        return handleSearchBooks(data);
    case RequestType::GetBookDetails:
        return handleGetBookDetails(data);
    case RequestType::AddBook:
        return handleAddBook(data, client);
    case RequestType::EditBook:
        return handleEditBook(data, client);
    case RequestType::InActiveBook:
        return handleInActiveBook(data, client);
    case RequestType::RestoreBook:
        return handleRestoreBook(data, client);
    case RequestType::ApplyDiscount:
        return handleApplyDiscount(data, client);
    case RequestType::AddReview:
        return handleAddReview(data, client);
    case RequestType::EditReview:
        return handleEditReview(data, client);
    case RequestType::DeleteReview:
        return handleDeleteReview(data, client);
    case RequestType::GetReviews:
        return handleGetReviews(data);
    case RequestType::GetTopSellingBooks:
        return handleGetTopSellingBooks();
    case RequestType::GetLeastSellingBooks:
        return handleGetLeastSellingBooks();
    case RequestType::GetNewestBooks:
        return handleGetNewestBooks();
    case RequestType::GetMostRatedBooks:
        return handleGetMostRatedBooks();
    case RequestType::GetFreeBooks:
        return handleGetFreeBooks();
    case RequestType::GetRecommendedBooks:
        return handleGetRecommendedBooks(client);
    case RequestType::GetBookFile:
        return handleGetBookFile(data, client);
    case RequestType::GetBookCover:
        return handleGetBookCover(data, client);
    default:
        return failure("Unknown book request");
    }
}