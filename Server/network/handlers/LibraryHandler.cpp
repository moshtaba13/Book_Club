#include "LibraryHandler.h"
#include "../ClientHandler.h"
#include "../../managers/PurchaseManager.h"
#include "../../repositories/SavedBookRepository.h"
#include "../../repositories/ShelfRepository.h"

QJsonObject LibraryHandler::handleGetPurchasedBooks(
    ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QVector<Purchase> purchases = PurchaseRepository::instance().findByUser(client->currentUser()->getId());

    QJsonArray arr;
    for (const Purchase &p : purchases)
        arr.append(purchaseToJson(p));

    QJsonObject result;
    result["purchases"] = arr;
    result["count"] = purchases.size();
    return success(result);
}

QJsonObject LibraryHandler::handleUpdateLastReadPage(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();
    int page = data["page"].toInt();

    if (!PurchaseRepository::instance().updateLastReadPage(client->currentUser()->getId(), bookId, page))
        return failure("Error Update on the study page");

    return success();
}

QJsonObject LibraryHandler::handleGetLastReadPage(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();
    int page = PurchaseRepository::instance().getLastReadPage(client->currentUser()->getId(), bookId);

    QJsonObject result;
    result["page"] = page;
    return success(result);
}

QJsonObject LibraryHandler::handleSaveBook(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();

    if (!SavedBookRepository::instance().save(client->currentUser()->getId(), bookId))
        return failure("Error saving book");

    return success();
}

QJsonObject LibraryHandler::handleRemoveSavedBook(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();

    if (!SavedBookRepository::instance().remove(client->currentUser()->getId(), bookId))
        return failure("Error deleting saved book");

    return success();
}

QJsonObject LibraryHandler::handleGetSavedBooks(ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QVector<Book> books = SavedBookRepository::instance().findByUser(client->currentUser()->getId());

    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    result["count"] = books.size();
    return success(result);
}

QJsonObject LibraryHandler::handleGetShelves(ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QVector<Shelf> shelves = ShelfRepository::instance().findByUser(client->currentUser()->getId());

    QJsonArray arr;
    for (const Shelf &s : shelves)
        arr.append(shelfToJson(s));

    QJsonObject result;
    result["shelves"] = arr;
    result["count"]   = shelves.size();
    return success(result);
}

QJsonObject LibraryHandler::handleAddShelf(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QString name = data["name"].toString();

    if (name.isEmpty())
        return invalidData("Shelf name cannot be empty.");

    if (!ShelfRepository::instance().addShelf(client->currentUser()->getId(), name))
        return alreadyExists("A shelf with this name already exists.");

    return success();
}

QJsonObject LibraryHandler::handleRemoveShelf(const QJsonObject &data,ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QString name = data["name"].toString();

    if (!ShelfRepository::instance().removeShelf(client->currentUser()->getId(), name))
        return failure("Error deleting shelf");

    return success();
}

QJsonObject LibraryHandler::handleRenameShelf(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QString oldName = data["old_name"].toString();
    QString newName = data["new_name"].toString();

    if (newName.isEmpty())
        return invalidData("The new name cannot be empty.");

    if (!ShelfRepository::instance().renameShelf(client->currentUser()->getId(), oldName, newName))
        return failure("Error renaming shelf");

    return success();
}

QJsonObject LibraryHandler::handleAddBookToShelf(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QString shelfName = data["shelf_name"].toString();
    int bookId = data["book_id"].toInt();

    if (!ShelfRepository::instance().addBook(client->currentUser()->getId(), shelfName, bookId))
        return failure("Error adding book to shelf");

    return success();
}

QJsonObject LibraryHandler::handleRemoveBookFromShelf(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QString shelfName = data["shelf_name"].toString();
    int bookId = data["book_id"].toInt();

    if (!ShelfRepository::instance().removeBook(client->currentUser()->getId(), shelfName, bookId))
        return failure("Error removing book from shelf");

    return success();
}

QJsonObject LibraryHandler::handleMoveBookBetweenShelves(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QString fromShelf = data["from_shelf"].toString();
    QString toShelf = data["to_shelf"].toString();
    int bookId = data["book_id"].toInt();

    if (!ShelfRepository::instance().moveBook(client->currentUser()->getId(), fromShelf, toShelf, bookId))
        return failure("Error in moving books between shelves");

    return success();
}

QJsonObject LibraryHandler::handle(RequestType type, const QJsonObject &data, ClientHandler *client)
{
    switch (type) {
    case RequestType::GetPurchasedBooks:
        return handleGetPurchasedBooks(client);
    case RequestType::UpdateLastReadPage:
        return handleUpdateLastReadPage(data, client);
    case RequestType::GetLastReadPage:
        return handleGetLastReadPage(data, client);
    case RequestType::SaveBook:
        return handleSaveBook(data, client);
    case RequestType::RemoveSavedBook:
        return handleRemoveSavedBook(data, client);
    case RequestType::GetSavedBooks:
        return handleGetSavedBooks(client);
    case RequestType::GetShelves:
        return handleGetShelves(client);
    case RequestType::AddShelf:
        return handleAddShelf(data, client);
    case RequestType::RemoveShelf:
        return handleRemoveShelf(data, client);
    case RequestType::RenameShelf:
        return handleRenameShelf(data, client);
    case RequestType::AddBookToShelf:
        return handleAddBookToShelf(data, client);
    case RequestType::RemoveBookFromShelf:
        return handleRemoveBookFromShelf(data, client);
    case RequestType::MoveBookBetweenShelves:
        return handleMoveBookBetweenShelves(data, client);
    default:
        return failure("Unknown library request");
    }
}