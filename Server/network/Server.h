#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class ClientHandler;

class Server : public QTcpServer
{
    Q_OBJECT

private:
    Server(QObject *parent = nullptr);
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    QVector<ClientHandler*> clients;

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public:
    static Server& instance();

    bool startServer(int port = 1234);
    void stopServer();

    int connectedClientsCount() const;

signals:
    void clientConnected(int clientId);
    void clientDisconnected(int clientId);
    void logMessage(const QString &message);


};

#endif