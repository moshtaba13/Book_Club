#include "AdminHandler.h"
#include "../ClientHandler.h"
#include "../../managers/UserManager.h"
#include "../../managers/BookManager.h"
#include "../../managers/FileManager.h"
#include "../../repositories/MemberRepository.h"
#include "../../repositories/BookRepository.h"

QJsonObject AdminHandler::handleGetAllMembers(ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();
        
    QVector<shared_ptr<Member>> members = MemberRepository::instance().findAll();

    QJsonArray arr;
    for (const auto &m : members)
        arr.append(memberToJson(*m));

    QJsonObject result;
    result["members"] = arr;
    result["count"] = members.size();
    return success(result);
}

QJsonObject AdminHandler::handleGetAllUsers(ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    QVector<shared_ptr<Member>> members = MemberRepository::instance().findAllUsers();

    QJsonArray arr;
    for (const auto &m : members)
        arr.append(memberToJson(*m));

    QJsonObject result;
    result["users"] = arr;
    result["count"] = members.size();
    return success(result);
}

QJsonObject AdminHandler::handleGetAllPublishers(ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    QVector<shared_ptr<Member>> members = MemberRepository::instance().findAllPublishers();

    QJsonArray arr;
    for (const auto &m : members)
        arr.append(memberToJson(*m));

    QJsonObject result;
    result["publishers"] = arr;
    result["count"]      = members.size();
    return success(result);
}

QJsonObject AdminHandler::handleSearchUsers(const QJsonObject &data, ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    QString keyword = data["keyword"].toString();

    QVector<shared_ptr<Member>> members = MemberRepository::instance().searchByUsername(keyword);

    QJsonArray arr;
    for (const auto &m : members)
        arr.append(memberToJson(*m));

    QJsonObject result;
    result["users"] = arr;
    result["count"] = members.size();
    return success(result);
}

QJsonObject AdminHandler::handleGetBlockedUsers(ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    QVector<shared_ptr<Member>> members = MemberRepository::instance().findBlockedUsers();

    QJsonArray arr;
    for (const auto &m : members)
        arr.append(memberToJson(*m));

    QJsonObject result;
    result["users"] = arr;
    result["count"] = members.size();
    return success(result);
}

QJsonObject AdminHandler::handleGetActiveUsers(ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    QVector<shared_ptr<Member>> members = MemberRepository::instance().findActiveUsers();

    QJsonArray arr;
    for (const auto &m : members)
        arr.append(memberToJson(*m));

    QJsonObject result;
    result["users"] = arr;
    result["count"] = members.size();
    return success(result);
}

QJsonObject AdminHandler::handleBlockUser(const QJsonObject &data, ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    int userId = data["user_id"].toInt();

    if (userId == client->currentUser()->getId())
        return failure("You cannot block your account.");

    if (!UserManager::instance().blockUser(userId))
        return failure("Error blocking user");

    return success();
}

QJsonObject AdminHandler::handleUnblockUser(const QJsonObject &data, ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    int userId = data["user_id"].toInt();

    if (!UserManager::instance().unblockUser(userId))
        return failure("Error unblocking user");

    return success();
}

QJsonObject AdminHandler::handleDeleteUser(const QJsonObject &data, ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    int userId = data["user_id"].toInt();

    if (userId == client->currentUser()->getId())
        return failure("You cannot delete your account");

    if (!MemberRepository::instance().remove(userId))
        return failure("Error deleting user");

    return success();
}

QJsonObject AdminHandler::handleGetAllBooksAdmin(ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    QVector<Book> books = BookRepository::instance().findAll();

    QJsonArray arr;
    for (const Book &b : books)
        arr.append(bookToJson(b));

    QJsonObject result;
    result["books"] = arr;
    result["count"] = books.size();
    return success(result);
}

QJsonObject AdminHandler::handleDeleteBookAdmin(const QJsonObject &data, ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();

    if (!BookRepository::instance().remove(bookId))
        return failure("Error deleting book");

    return success();
}

QJsonObject AdminHandler::handleEditBookAdmin(const QJsonObject &data, ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    Book book = BookRepository::instance().findById(data["id"].toInt());

    if (book.getId() == 0)
        return notFound("Book not found.");

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

QJsonObject AdminHandler::handleDeleteReviewAdmin(const QJsonObject &data, ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    if (!BookRepository::instance().removeReview(data["book_id"].toInt(), client->currentUser()->getId()))
        return failure("Error deleting review");

    return success();
}

QJsonObject AdminHandler::handle(RequestType type,
                                  const QJsonObject &data,
                                  ClientHandler *client)
{
    if (!isAdmin(client))
        return unauthorized();

    switch (type) {
    case RequestType::GetAllUsers:
        return handleGetAllUsers(client);
    case RequestType::GetAllPublishers:
        return handleGetAllPublishers(client);
    case RequestType::SearchUsers:
        return handleSearchUsers(data, client);
    case RequestType::GetBlockedUsers:
        return handleGetBlockedUsers(client);
    case RequestType::GetUnblockedUser:
        return handleGetActiveUsers(client);
    case RequestType::BlockUser:
        return handleBlockUser(data, client);
    case RequestType::UnblockUser:
        return handleUnblockUser(data, client);
    case RequestType::DeleteUser:
        return handleDeleteUser(data, client);
    case RequestType::GetAllBooksAdmin:
        return handleGetAllBooksAdmin(client);
    case RequestType::DeleteBookAdmin:
        return handleDeleteBookAdmin(data, client);
    case RequestType::EditBookAdmin:
        return handleEditBookAdmin(data, client);
    case RequestType::DeleteReviewAdmin:
        return handleDeleteReviewAdmin(data, client);
    default:
        return failure("Unknown admin request");
    }
}