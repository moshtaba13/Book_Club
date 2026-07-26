#include "BaseHandler.h"
#include "../ClientHandler.h"
#include "../../../Common/users/User.h"
#include "../../../Common/users/Publisher.h"

QJsonObject BaseHandler::success(const QJsonObject &data)
{
    QJsonObject response;
    response["status"] = "Success";
    if (!data.isEmpty())
        response["data"] = data;
    return response;
}

QJsonObject BaseHandler::failure(const QString &message)
{
    QJsonObject response;
    response["status"]  = "Failure";
    response["message"] = message;
    return response;
}

QJsonObject BaseHandler::unauthorized()
{
    QJsonObject response;
    response["status"]  = "Unauthorized";
    response["message"] = "Unauthorized access";
    return response;
}

QJsonObject BaseHandler::notFound(const QString &message)
{
    QJsonObject response;
    response["status"]  = "NotFound";
    response["message"] = message;
    return response;
}

QJsonObject BaseHandler::alreadyExists(const QString &message)
{
    QJsonObject response;
    response["status"]  = "AlreadyExists";
    response["message"] = message;
    return response;
}

QJsonObject BaseHandler::invalidData(const QString &message)
{
    QJsonObject response;
    response["status"]  = "InvalidData";
    response["message"] = message;
    return response;
}

bool BaseHandler::isLoggedIn(ClientHandler *client) {
    return client->currentUser() != nullptr;
}

bool BaseHandler::isUser(ClientHandler *client) {
    return isLoggedIn(client) && client->currentUser()->role() == "User";
}

bool BaseHandler::isPublisher(ClientHandler *client) {
    return isLoggedIn(client) && client->currentUser()->role() == "Publisher";
}

bool BaseHandler::isAdmin(ClientHandler *client) {
    return isLoggedIn(client) && client->currentUser()->role() == "Admin";
}

QJsonObject BaseHandler::bookToJsonFull(const Book &book)
{
    QJsonObject obj;
    obj["id"] = book.getId();
    obj["title"] = book.getTitle();
    obj["author"] = book.getAuthor();
    obj["genre"] = genreToString(book.getGenre());
    obj["description"] = book.getDiscription();
    obj["price"] = book.getPrice();
    obj["discount"] = book.getDiscount();
    obj["final_price"] = book.getFinalPrice();
    obj["is_active"] = book.getisActive();
    obj["sales_count"] = book.getSalesCount();
    obj["average_rating"] = book.getAverageRating();
    obj["publisher_username"] = book.getPublisherUsername();
    obj["publish_date"] = book.getPublishDate().toString(Qt::ISODate);

    QJsonArray reviewsArray;
    for (const Review &r : book.getReviews())
        reviewsArray.append(reviewToJson(r));
    obj["reviews"] = reviewsArray;

    return obj;
}

QJsonObject BaseHandler::bookToJson(const Book &book)
{
    QJsonObject obj;
    obj["id"] = book.getId();
    obj["title"] = book.getTitle();
    obj["author"] = book.getAuthor();
    obj["genre"] = genreToString(book.getGenre());
    obj["price"] = book.getPrice();
    obj["discount"] = book.getDiscount();
    obj["final_price"] = book.getFinalPrice();
    obj["publisher_username"] = book.getPublisherUsername();

    return obj;
}

QJsonObject BaseHandler::memberToJsonFull(const Member &member)
{
    QJsonObject obj;
    obj["id"] = member.getId();
    obj["username"] = member.getUsername();
    obj["role"] = member.role();
    obj["is_blocked"] = member.isBlocked();
    obj["register_date"] = member.getRegisterDate().toString(Qt::ISODate);
    obj["unread_notifications"] = member.unreadNotificationsCount();

    QJsonArray notifsArray; 
    for (const Notification &n : member.getNotifications())
        notifsArray.append(notificationToJson(n));
    
    obj["notifications"] = notifsArray;

    if (member.role() == "User") {
        const User *user = dynamic_cast<const User*>(&member);
        if (user) {
            obj["is_first_login"]  = user->isFirstLogin();
            obj["purchases_count"] = user->getPurchasecount();

            QJsonArray genresArray;
            for (const genre &g : user->getfavoriteGenres())
                genresArray.append(genreToString(g));
            obj["favorite_genres"] = genresArray;

            QJsonArray purchaseArray;
            for (const Purchase &p : user->getpurchasedBooks())
                purchaseArray.append(purchaseToJson(p));
            obj["purchased_book"] = purchaseArray;

            QJsonArray savedArray;
            for (const Book &b : user->getSavedBooks())
                savedArray.append(bookToJson(b));
            obj["saved_book"] = savedArray;

            obj["cart"] = cartToJson(user->getCart());

            QJsonArray shelvesArray;
            for (const Shelf &s : user->getShelves())
                shelvesArray.append(shelfToJson(s));
            obj["shelves"] = shelvesArray;

        }
    }

    if (member.role() == "Publisher") {
        const Publisher *pub = dynamic_cast<const Publisher*>(&member);
        if (pub) {
            obj["total_revenue"] = pub->getTotalRevenue();
            obj["published_books_count"] = pub->numberOfPublishedBook();

            QJsonArray booksArray;
            for (const Book &b : pub->getPublishedBooks())
                booksArray.append(bookToJson(b));
            obj["published_books"] = booksArray;
        }
    }

    return obj;
}

QJsonObject BaseHandler::memberToJson(const Member &member)
{
    QJsonObject obj;

    obj["id"] = member.getId();
    obj["username"] = member.getUsername();
    obj["role"] = member.role();
    obj["is_blocked"] = member.isBlocked();
    obj["register_date"] = member.getRegisterDate().toString(Qt::ISODate);

    if (member.role() == "User") {
        const User *user = dynamic_cast<const User*>(&member);
        if (user) {
            obj["is_first_login"]  = user->isFirstLogin();
            obj["purchases_count"] = user->getPurchasecount();

            QJsonArray genresArray;
            for (const genre &g : user->getfavoriteGenres())
                genresArray.append(genreToString(g));
            obj["favorite_genres"] = genresArray;
        }
    }

    if (member.role() == "Publisher") {
        const Publisher *pub = dynamic_cast<const Publisher*>(&member);
        if (pub) {
            obj["total_revenue"] = pub->getTotalRevenue();
            obj["published_books_count"] = pub->numberOfPublishedBook();
        }
    }

    obj["unread_notifications"] = member.unreadNotificationsCount();

    return obj;
}

QJsonObject BaseHandler::reviewToJson(const Review &review)
{
    QJsonObject obj;
    obj["user_id"] = review.getUserId();
    obj["Username"] = review.getUsername();
    obj["stars"] = review.getStars();
    obj["comment"] = review.getComment();
    obj["timestamp"] = review.getTimestamp().toString(Qt::ISODate);
    if (review.getLastEdited().isValid())
        obj["last_edited"] = review.getLastEdited().toString(Qt::ISODate);
    else
        obj["last_edited"] = QJsonValue::Null;
    return obj;
}

QJsonObject BaseHandler::purchaseToJson(const Purchase &purchase)
{
    QJsonObject obj;
    obj["book"] = bookToJson(purchase.getBook());
    obj["price_paid"] = purchase.getPricePaid();
    obj["purchase_date"] = purchase.getPurchaseDate().toString(Qt::ISODate);
    obj["last_read_page"] = purchase.getLastReadPage();
    return obj;
}

QJsonObject BaseHandler::notificationToJson(const Notification &n)
{
    QJsonObject obj;
    obj["id"] = n.getId();
    obj["message"] = n.getMessage();
    obj["is_read"] = n.isRead();
    obj["created_at"] = n.getCreatedAt().toString(Qt::ISODate);
    return obj;
}

QJsonObject BaseHandler::shelfToJson(const Shelf &shelf)
{
    QJsonObject obj;
    obj["name"] = shelf.getName();

    QJsonArray booksArray;
    for (const Book &b : shelf.getBooks())
        booksArray.append(bookToJson(b));
    obj["books"] = booksArray;

    return obj;
}

QJsonObject BaseHandler::cartToJson(const Cart &cart)
{
    QJsonObject obj;

    QJsonArray itemsArray;
    for (const Book &b : cart.getItems())
        itemsArray.append(bookToJson(b));
    obj["items"] = itemsArray;

    obj["item_count"] = cart.getNumberofitems();
    obj["total_price"] = cart.getTotalPrice();
    obj["total_discount"] = cart.getTotalDiscount();
    obj["final_price"] = cart.getFinalPrice();
    obj["discount_percentage"] = cart.getTotaldiscountPercentage();
    obj["is_empty"] = cart.isEmpty();

    return obj;
}