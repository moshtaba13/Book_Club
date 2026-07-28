#include "Server.h"
#include "ClientHandler.h"
#include "../Database.h"
#include <QProcessEnvironment>

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

    // Allow overriding the port the same way the client can point at a
    // custom host/port, instead of only ever trying the hardcoded default.
    bool portOverridden = false;
    int envPort = qEnvironmentVariableIntValue("BOOKCLUB_SERVER_PORT", &portOverridden);
    if (portOverridden && envPort > 0)
        port = envPort;

    if (!listen(QHostAddress::Any, port)) {
        // errorString() tells you *why* the bind failed - almost always
        // "Address already in use" (another server instance, or a previous
        // run that didn't shut down cleanly, is still holding the port) but
        // could also be a permissions/firewall issue. Surfacing it saves a
        // lot of guessing versus the old generic message.
        emit logMessage("Server failed to start on port "
                        + QString::number(port) + ": " + errorString());
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