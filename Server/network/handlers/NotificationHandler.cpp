#include "NotificationHandler.h"
#include "../ClientHandler.h"
#include "../../repositories/NotificationRepository.h"


QJsonObject NotificationHandler::handleGetNotifications(ClientHandler *client)
{
    QVector<Notification> notifications = NotificationRepository::instance()
        .findByUser(client->currentUser()->getId());

    QJsonArray arr;
    for (const Notification &n : notifications)
        arr.append(notificationToJson(n));

    QJsonObject result;
    result["notifications"] = arr;
    result["count"] = notifications.size();
    result["unread_count"] = NotificationRepository::instance().unreadCount(client->currentUser()->getId());
    return success(result);
}

QJsonObject NotificationHandler::handleMarkNotificationRead(const QJsonObject &data, ClientHandler *client)
{
    int notifId = data["notification_id"].toInt();

    if (!NotificationRepository::instance().markAsRead(notifId))
        return failure("Error marking notification");

    return success();
}

QJsonObject NotificationHandler::handleMarkAllNotificationsRead(ClientHandler *client)
{
    if (!NotificationRepository::instance().markAllAsRead(client->currentUser()->getId()))
        return failure("Error marking all notification");

    return success();
}


QJsonObject NotificationHandler::handle(RequestType type,
                                         const QJsonObject &data,
                                         ClientHandler *client)
{
    if (!isLoggedIn(client))
        return unauthorized();

    switch (type) {
    case RequestType::GetNotifications:
        return handleGetNotifications(client);
    case RequestType::MarkNotificationRead:
        return handleMarkNotificationRead(data, client);
    case RequestType::MarkAllNotificationsRead:
        return handleMarkAllNotificationsRead(client);
    default:
        return failure("Unknown notification request");
    }
}