#include "modelserializer.h"

#include <QJsonArray>
#include <QDateTime>

namespace ModelSerializer {

Book bookFromJson(const QJsonObject &obj)
{
    Book book;
    book.setId(obj.value("id").toInt());
    book.setTitle(obj.value("title").toString());
    book.setAuthor(obj.value("author").toString());
    book.setGenre(stringToGenre(obj.value("genre").toString()));
    book.setDescription(obj.value("description").toString());
    book.setPrice(obj.value("price").toDouble());
    book.setDiscount(obj.value("discount").toDouble());
    book.setisActive(obj.value("is_active").toBool(true));
    book.setSalesCount(obj.value("sales_count").toInt());
    book.setAverageRating(obj.value("average_rating").toDouble());
    book.setPublisherUsername(obj.value("publisher_username").toString());

    if (obj.contains("publish_date"))
        book.setPublishDate(QDateTime::fromString(obj.value("publish_date").toString(), Qt::ISODate));

    for (const QJsonValue &v : obj.value("reviews").toArray())
        book.addReview(reviewFromJson(v.toObject()));

    return book;
}

Review reviewFromJson(const QJsonObject &obj)
{
    Review review(
        obj.value("user_id").toInt(),
        obj.value("stars").toInt(),
        obj.value("comment").toString()
        );
    review.setUsername(obj.value("Username").toString());

    if (obj.value("last_edited").isString())
        review.Edit(review.getStars(), review.getComment());

    return review;
}

Purchase purchaseFromJson(const QJsonObject &obj)
{
    Purchase purchase(
        bookFromJson(obj.value("book").toObject()),
        obj.value("price_paid").toDouble(),
        QDateTime::fromString(obj.value("purchase_date").toString(), Qt::ISODate)
        );
    purchase.setLastReadPage(obj.value("last_read_page").toInt());
    return purchase;
}

Shelf shelfFromJson(const QJsonObject &obj)
{
    Shelf shelf(obj.value("name").toString());
    for (const QJsonValue &v : obj.value("books").toArray())
        shelf.addBook(bookFromJson(v.toObject()));
    return shelf;
}

Notification notificationFromJson(const QJsonObject &obj)
{
    Notification notification(obj.value("message").toString());
    notification.setId(obj.value("id").toInt());
    notification.setCreatedAt(QDateTime::fromString(obj.value("created_at").toString(), Qt::ISODate));
    if (obj.value("is_read").toBool())
        notification.markAsRead();
    return notification;
}

Cart cartFromJson(const QJsonObject &obj)
{
    Cart cart;
    QVector<Book> items;
    for (const QJsonValue &v : obj.value("items").toArray())
        items.append(bookFromJson(v.toObject()));
    cart.loadItems(items);
    return cart;
}

void applyMemberFields(Member &member, const QJsonObject &obj)
{
    member.setId(obj.value("id").toInt());
    member.setUsername(obj.value("username").toString());
    member.setBlocked(obj.value("is_blocked").toBool());

    if (obj.contains("register_date"))
        member.setRegisterDate(QDateTime::fromString(obj.value("register_date").toString(), Qt::ISODate));

    for (const QJsonValue &v : obj.value("notifications").toArray())
        member.addNotification(notificationFromJson(v.toObject()));
}

User userFromJson(const QJsonObject &obj)
{
    User user;
    applyMemberFields(user, obj);

    user.setFirstLogin(obj.value("is_first_login").toBool());
    user.setPurchaseCount(obj.value("purchases_count").toInt());

    QVector<genre> genres;
    for (const QJsonValue &v : obj.value("favorite_genres").toArray())
        genres.append(stringToGenre(v.toString()));
    user.setFavoriteGenres(genres);

    for (const QJsonValue &v : obj.value("purchased_book").toArray())
        user.addPurchasedBook(purchaseFromJson(v.toObject()));

    for (const QJsonValue &v : obj.value("saved_book").toArray())
        user.addSavedBook(bookFromJson(v.toObject()));

    for (const QJsonValue &v : obj.value("shelves").toArray())
        user.addShelf(shelfFromJson(v.toObject()));

    return user;
}

Publisher publisherFromJson(const QJsonObject &obj)
{
    Publisher publisher;
    applyMemberFields(publisher, obj);

    publisher.setTotalRevenue(obj.value("total_revenue").toDouble());

    for (const QJsonValue &v : obj.value("published_books").toArray())
        publisher.addPublishedBook(bookFromJson(v.toObject()));

    return publisher;
}

}
