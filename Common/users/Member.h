#ifndef MEMBER_H
#define MEMBER_H

#include <Qstring>
#include <QDateTime>
#include <QVector>

#include "../models/Notification.h"

class Member
{
private:
    int ID;
    QString username;
    QString password;
    bool blocked;
    QString securityAnswer;
    QDateTime registerDate;
    QVector<Notification> notifications;
public:
    Member();
    Member(const QString& username, const QString& password, const QString& securityAnswer);
    virtual ~Member() = default;

    int getId() const;
    void setId(int id);
    QString getUsername() const;
    void setUsername(const QString& name);
    QString getPassword() const;
    void setPassword(const QString& pass);
    QString getSecurityAnswer() const;
    bool isBlocked() const;
    void setBlocked(bool block);
    QDateTime getRegisterDate() const;
    void setRegisterDate(const QDateTime &date);

    const QVector<Notification>& getNotifications() const;
    void addNotification(const Notification &notification);
    void markNotificationRead(int notificationId);
    void markAllNotificationsRead();
    int unreadNotificationsCount() const;

    virtual QString role() const = 0;
};

#endif