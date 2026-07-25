#include "member.h"

Member::Member():ID(0), blocked(false) {};
Member::Member(const QString& username, const QString& password, const QString& securityAnswer): 
ID(0), username(username), password(password), securityAnswer(securityAnswer), blocked(false){}

int Member::getId() const{return ID;}
void Member::setId(int id) {ID = id;}
QString Member::getUsername() const{return username;}
void Member::setUsername(const QString& name) {username = name;}
QString Member::getPassword() const{return password;}
void Member::setPassword(const QString& pass) {password = pass;}
QString Member::getSecurityAnswer() const{return securityAnswer;}
bool Member::isBlocked() const{return blocked;}
void Member::setBlocked(bool block) {blocked = block;} 
QDateTime Member::getRegisterDate() const { return registerDate; }
void Member::setRegisterDate(const QDateTime &date) { registerDate = date; }

const QVector<Notification>& Member::getNotifications() const{return notifications;}

void Member::addNotification(const Notification &notification) {
    notifications.append(notification);
}

void Member::markNotificationRead(int notificationId){
    for (Notification &n : notifications) {
        if (n.getId() == notificationId) {
            n.markAsRead();
            break;
        }
    }
}

void Member::markAllNotificationsRead() {
    for (Notification &n : notifications)
        n.markAsRead();
}

int Member::unreadNotificationsCount() const{
    int count = 0;
    for (const Notification &n : notifications)
        if (!n.isRead())
            count++;
    return count;
}
QString Member::getEmail() const
{
    return email;
}

void Member::setEmail(const QString &newEmail)
{
    email = newEmail;
}
