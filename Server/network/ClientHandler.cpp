#include "ClientHandler.h"
#include "handlers/AuthHandler.h"
#include "handlers/BookHandler.h"
#include "handlers/CartHandler.h"
#include "handlers/LibraryHandler.h"
#include "handlers/ProfileHandler.h"
#include "handlers/AdminHandler.h"
#include "handlers/NotificationHandler.h"


int ClientHandler::nextClientId = 0;

ClientHandler::ClientHandler(qintptr socketDescriptor, QObject *parent)
    : QObject(parent)
    , currUser(nullptr)
    , clientID(++nextClientId)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead,this, &ClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected,this, &ClientHandler::onDisconnected);
}

ClientHandler::~ClientHandler() {
    if (socket->isOpen())
        socket->close();
}

void ClientHandler::disconnectClient() {
    socket->disconnectFromHost();
}

int ClientHandler::clientId() const { return clientID; }

shared_ptr<Member> ClientHandler::currentUser() const{return currUser;}

void ClientHandler::setCurrentUser(shared_ptr<Member> user) {currUser = user;}

void ClientHandler::sendNotification(const QJsonObject &data) {sendResponse(data);}

void ClientHandler::onReadyRead()
{
    buffer.append(socket->readAll());

    while (buffer.size() >= 4) {

        quint32 messageLength;
        QDataStream stream(buffer.left(4));
        stream >> messageLength;

        if (buffer.size() < 4 + (int)messageLength)
            break;

        QByteArray jsonData = buffer.mid(4, messageLength);
        buffer.remove(0, 4 + messageLength);

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isObject()) {
            QJsonObject err;
            err["status"]  = "Failure";
            err["message"] = "Invalid JSON format";
            sendResponse(err);
            return;
        }
        processRequest(doc.object()); 
    }

}

void ClientHandler::onDisconnected()
{
    emit logMessage("Client " + QString::number(clientID) + " disconnected");
    emit disconnected();
}

void ClientHandler::processRequest(const QJsonObject &request)
{
    QString typeStr = request["type"].toString();
    QJsonObject data = request["data"].toObject();
    RequestType type = stringToRequestType(typeStr);

    emit logMessage("Client " + QString::number(clientID) + " → " + typeStr);

    QJsonObject response;

    switch (type) {

    case RequestType::Login:
    case RequestType::Register:
    case RequestType::RegisterPublisher:
    case RequestType::ResetPassword:
    case RequestType::Logout:
        response = AuthHandler::handle(type, data, this);
        break;

    case RequestType::GetAllBooks:
    case RequestType::GetBooksByGenre:
    case RequestType::GetBooksByPublisher:
    case RequestType::SearchBooks:
    case RequestType::GetBookDetails:
    case RequestType::AddBook:
    case RequestType::EditBook:
    case RequestType::InActiveBook:
    case RequestType::RestoreBook:
    case RequestType::ApplyDiscount:
    case RequestType::AddReview:
    case RequestType::EditReview:
    case RequestType::DeleteReview:
    case RequestType::GetReviews:
    case RequestType::GetTopSellingBooks:
    case RequestType::GetLeastSellingBooks:
    case RequestType::GetNewestBooks:
    case RequestType::GetMostRatedBooks:
    case RequestType::GetFreeBooks:
    case RequestType::GetRecommendedBooks:
        response = BookHandler::handle(type, data, this);
        break;

    case RequestType::AddToCart:
    case RequestType::RemoveFromCart:
    case RequestType::GetCart:
    case RequestType::Checkout:
        response = CartHandler::handle(type, data, this);
        break;

    case RequestType::GetPurchasedBooks:
    case RequestType::UpdateLastReadPage:
    case RequestType::GetLastReadPage:
    case RequestType::SaveBook:
    case RequestType::RemoveSavedBook:
    case RequestType::GetSavedBooks:
    case RequestType::GetShelves:
    case RequestType::AddShelf:
    case RequestType::RemoveShelf:
    case RequestType::RenameShelf:
    case RequestType::AddBookToShelf:
    case RequestType::RemoveBookFromShelf:
    case RequestType::MoveBookBetweenShelves:
        response = LibraryHandler::handle(type, data, this);
        break;


    case RequestType::GetProfile:
    case RequestType::ChangeUsername:
    case RequestType::ChangePassword:
    case RequestType::UpdateFavoriteGenres:
        response = ProfileHandler::handle(type, data, this);
        break;


    case RequestType::GetAllMembers:
    case RequestType::GetAllUsers:
    case RequestType::GetAllPublishers:
    case RequestType::SearchUsers:
    case RequestType::GetBlockedUsers:
    case RequestType::BlockUser:
    case RequestType::UnblockUser:
    case RequestType::DeleteUser:
    case RequestType::GetAllBooksAdmin:
    case RequestType::DeleteBookAdmin:
    case RequestType::EditBookAdmin:
    case RequestType::DeleteReviewAdmin:
        response = AdminHandler::handle(type, data, this);
        break;


    case RequestType::GetNotifications:
    case RequestType::MarkNotificationRead:
    case RequestType::MarkAllNotificationsRead:
        response = NotificationHandler::handle(type, data, this);
        break;

    default:
        response["status"]  = "Failure";
        response["message"] = "Unknown request type";
    }

    sendResponse(response);
}

void ClientHandler::sendResponse(const QJsonObject &response)
{
    QJsonDocument doc(response);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    QByteArray lengthBytes;
    QDataStream stream(&lengthBytes, QIODevice::WriteOnly);
    stream << (quint32)jsonData.size();

    socket->write(lengthBytes);
    socket->write(jsonData);
    socket->flush();
}