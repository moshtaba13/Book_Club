#ifndef CARTHANDLER_H
#define CARTHANDLER_H

#include "BaseHandler.h"
#include "../../../Common/protocol.h"

class CartHandler : public BaseHandler
{
private:
    static QJsonObject handleAddToCart(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleRemoveFromCart(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleGetCart(ClientHandler *client);
    static QJsonObject handleCheckout(ClientHandler *client);
public:
    static QJsonObject handle(RequestType type,
                              const QJsonObject &data,
                              ClientHandler *client);

};

#endif