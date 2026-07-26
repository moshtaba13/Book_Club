#include "CartHandler.h"
#include "../ClientHandler.h"
#include "../../managers/PurchaseManager.h"

QJsonObject CartHandler::handleAddToCart(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();

    if (!PurchaseManager::instance().addToCart(client->currentUser()->getId(), bookId))
        return failure("Error adding to cart");

    return success();
}

QJsonObject CartHandler::handleRemoveFromCart(const QJsonObject &data, ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    int bookId = data["book_id"].toInt();

    if (!CartRepository::instance().removeBook(client->currentUser()->getId(), bookId))
        return failure("Error removing from cart");

    return success();
}

QJsonObject CartHandler::handleGetCart(ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();
    Cart cart = CartRepository::instance().loadCart(client->currentUser()->getId());

    return success(BaseHandler::cartToJson(cart));
}

QJsonObject CartHandler::handleCheckout(ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    if (!PurchaseManager::instance().checkout(client->currentUser()->getId()))
        return failure("Error in finalizing purchase");

    return success();
}

QJsonObject CartHandler::handle(RequestType type, const QJsonObject &data, ClientHandler *client)
{
    switch (type) {
    case RequestType::AddToCart:
        return handleAddToCart(data, client);
    case RequestType::RemoveFromCart:
        return handleRemoveFromCart(data, client);
    case RequestType::GetCart:
        return handleGetCart(client);
    case RequestType::Checkout:
        return handleCheckout(client);
    default:
        return failure("Unknown cart request");
    }
}