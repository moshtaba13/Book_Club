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

// Client-side counterpart to the server's BaseHandler::*ToJson helpers -
// turns the JSON objects the server sends back into the shared Common model
// objects the UI already knows how to render.
namespace ModelSerializer {

Book bookFromJson(const QJsonObject &obj);
Review reviewFromJson(const QJsonObject &obj);
Purchase purchaseFromJson(const QJsonObject &obj);
Shelf shelfFromJson(const QJsonObject &obj);
Notification notificationFromJson(const QJsonObject &obj);
Cart cartFromJson(const QJsonObject &obj);

// Fills in the fields shared by every member role (id, username, blocked
// state, register date, notifications). Safe to call on User/Publisher/Admin.
void applyMemberFields(Member &member, const QJsonObject &obj);

// Handles both the full login/registration payload (memberToJsonFull on the
// server) and the lighter GetProfile payload (memberToJson) - missing keys
// are simply left at their default value.
User userFromJson(const QJsonObject &obj);
Publisher publisherFromJson(const QJsonObject &obj);

}

#endif // MODELSERIALIZER_H
