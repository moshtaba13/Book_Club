#ifndef NOTIFICATIONREPOSITORY_H
#define NOTIFICATIONREPOSITORY_H

#include <QVector>

#include "../Database.h"
#include "../../Common/models/Notification.h"

class NotificationRepository
{
private:
    NotificationRepository() = default;
    NotificationRepository(const NotificationRepository&) = delete;
    NotificationRepository& operator=(const NotificationRepository&) = delete;

    Notification mapToNotification(QSqlQuery &query);
public:
    static NotificationRepository& instance();

    bool save(int userId, const Notification &notification);
    bool markAsRead(int notificationId);
    bool markAllAsRead(int userId);

    QVector<Notification> findByUser(int userId);

    int unreadCount(int userId);


};

#endif