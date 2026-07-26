#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <memory>

#include "../../../Common/users/Member.h"
#include "../../../Common/users/User.h"
#include "../../../Common/users/Publisher.h"
#include "../../../Common/models/Book.h"
#include "../../../Common/models/Review.h"
#include "../../../Common/models/Purchase.h"
#include "../../../Common/models/Notification.h"
#include "../../../Common/models/Shelf.h"

class ClientHandler;

class BaseHandler
{
protected:
    static QJsonObject success(const QJsonObject &data = QJsonObject());
    static QJsonObject failure(const QString &message);
    static QJsonObject unauthorized();
    static QJsonObject notFound(const QString &message);
    static QJsonObject alreadyExists(const QString &message);
    static QJsonObject invalidData(const QString &message);

    static bool isLoggedIn(ClientHandler *client);

    static bool isUser(ClientHandler *client);
    static bool isPublisher(ClientHandler *client);
    static bool isAdmin(ClientHandler *client);
    
    static QJsonObject bookToJsonFull(const Book &book);
    static QJsonObject bookToJson(const Book &book);
    static QJsonObject memberToJsonFull(const Member &member);
    static QJsonObject memberToJson(const Member &member);
    static QJsonObject reviewToJson(const Review &review);
    static QJsonObject purchaseToJson(const Purchase &purchase);
    static QJsonObject notificationToJson(const Notification &n);
    static QJsonObject shelfToJson(const Shelf &shelf);
    static QJsonObject cartToJson(const Cart &cart);
};

#endif