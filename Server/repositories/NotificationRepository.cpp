#include "NotificationRepository.h"

Notification NotificationRepository::mapToNotification(QSqlQuery &query) {
    Notification notification(query.value("message").toString());
    notification.setId(query.value("id").toInt());
    notification.setCreatedAt(QDateTime::fromString(query.value("created_at").toString(), Qt::ISODate));

    if (query.value("is_read").toInt() == 1)
        notification.markAsRead();

    return notification;
}

NotificationRepository& NotificationRepository::instance() {
    static NotificationRepository instance;
    return instance;
}

bool NotificationRepository::save(int userId, const Notification &notification)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO notifications "
        "(user_id, message, is_read, created_at) "
        "VALUES (:uid, :message, :is_read, :created_at)"
    );
    query.bindValue(":uid",        userId);
    query.bindValue(":message",    notification.getMessage());
    query.bindValue(":is_read",    notification.isRead() ? 1 : 0);
    query.bindValue(":created_at", notification.getCreatedAt().toString(Qt::ISODate));

    if (!query.exec()) {
        qWarning() << "NotificationRepository::save error:"<< query.lastError().text();
        return false;
    }

    const_cast<Notification&>(notification).setId(query.lastInsertId().toInt());

    return true;
}

bool NotificationRepository::markAsRead(int notificationId)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE notifications SET is_read = 1 WHERE id = :id");
    query.bindValue(":id", notificationId);

    if (!query.exec()) {
        qWarning() << "NotificationRepository::markAsRead error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool NotificationRepository::markAllAsRead(int userId) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE notifications SET is_read = 1 WHERE user_id = :uid");
    query.bindValue(":uid", userId);

    if (!query.exec()) {
        qWarning() << "NotificationRepository::markAllAsRead error:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<Notification> NotificationRepository::findByUser(int userId) {
    QVector<Notification> notifications;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT * FROM notifications "
        "WHERE user_id = :uid "
        "ORDER BY created_at DESC"
    );
    query.bindValue(":uid", userId);

    if (!query.exec())
        return notifications;

    while (query.next())
        notifications.append(mapToNotification(query));

    return notifications;
}

int NotificationRepository::unreadCount(int userId) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT COUNT(*) FROM notifications "
        "WHERE user_id = :uid AND is_read = 0"
    );
    query.bindValue(":uid", userId);
    query.exec();

    if (query.next())
        return query.value(0).toInt();
    return 0;
}

