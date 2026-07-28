#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <memory>

#include "../../Common/users/Member.h"
#include "../../Common/protocol.h"

using namespace std;

class ClientHandler : public QObject
{
    Q_OBJECT

private:
    void processRequest(const QJsonObject &request);
    void sendResponse(const QJsonObject &response);

    QTcpSocket *socket;
    shared_ptr<Member> currUser;
    int clientID;
    QByteArray buffer;
    static int nextClientId;

public:
    explicit ClientHandler(qintptr socketDescriptor, QObject *parent = nullptr);
    ~ClientHandler();

    void disconnectClient();
    int clientId() const;

    void sendNotification(const QJsonObject &data);

    shared_ptr<Member> currentUser() const;
    void setCurrentUser(shared_ptr<Member> user);

signals:
    void disconnected();
    void logMessage(const QString &message);

private slots:
    void onReadyRead();
    void onDisconnected();

};

#endif