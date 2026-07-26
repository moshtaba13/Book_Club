#include "Server.h"
#include "ClientHandler.h"
#include "../Database.h"

Server& Server::instance() {
    static Server instance;
    return instance;
}

Server::Server(QObject *parent): QTcpServer(parent){}

bool Server::startServer(int port)
{
    if (!Database::instance().connect()) {
        emit logMessage("Database connection failed!");
        return false;
    }

    if (!listen(QHostAddress::Any, port)) {
        emit logMessage("Server failed to start on port "
                       + QString::number(port));
        return false;
    }

    emit logMessage("Server started on port "
                   + QString::number(port));
    return true;
}

void Server::stopServer()
{
    for (ClientHandler *client : clients)
        client->disconnectClient();

    clients.clear();
    close();
    Database::instance().close();
    emit logMessage("Server stopped.");
}

int Server::connectedClientsCount() const
{
    return clients.size();
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    ClientHandler *client = new ClientHandler(socketDescriptor, this);

    connect(client, &ClientHandler::disconnected,
            this, [this, client]() {
                clients.removeOne(client);
                emit clientDisconnected(client->clientId());
                emit logMessage("Client disconnected. Online: " + QString::number(clients.size()));
                client->deleteLater();
            });

    connect(client, &ClientHandler::logMessage, this, &Server::logMessage);

    clients.append(client);
    emit clientConnected(client->clientId());
    emit logMessage("New client connected. Online: " + QString::number(clients.size()));
}