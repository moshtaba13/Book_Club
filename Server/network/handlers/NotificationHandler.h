#ifndef NOTIFICATIONHANDLER_H
#define NOTIFICATIONHANDLER_H

#include "BaseHandler.h"
#include "../../../Common/Protocol.h"

class NotificationHandler : public BaseHandler
{
private:
    static QJsonObject handleGetNotifications(ClientHandler *client);
    static QJsonObject handleMarkNotificationRead(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleMarkAllNotificationsRead(ClientHandler *client);
    
public:
    static QJsonObject handle(RequestType type,
                              const QJsonObject &data,
                              ClientHandler *client);
};

#endif