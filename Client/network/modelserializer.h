#ifndef MODELSERIALIZER_H
#define MODELSERIALIZER_H

#include <QJsonObject>
#include <QVector>

#include "Book.h"
#include "Review.h"
#include "Purchase.h"
#include "Shelf.h"
#include "Notification.h"
#include "Cart.h"
#include "User.h"
#include "Publisher.h"
#include "protocol.h"


namespace ModelSerializer {

Book bookFromJson(const QJsonObject &obj);
Review reviewFromJson(const QJsonObject &obj);
Purchase purchaseFromJson(const QJsonObject &obj);
Shelf shelfFromJson(const QJsonObject &obj);
Notification notificationFromJson(const QJsonObject &obj);
Cart cartFromJson(const QJsonObject &obj);


void applyMemberFields(Member &member, const QJsonObject &obj);


User userFromJson(const QJsonObject &obj);
Publisher publisherFromJson(const QJsonObject &obj);

}

#endif // MODELSERIALIZER_H
