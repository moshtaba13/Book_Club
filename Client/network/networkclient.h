#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QString>

#include "protocol.h"

// Persistent, synchronous TCP client for talking to the BookClub server.
// The server ties a logged-in session to the TCP connection itself, so a
// single connection is opened once and reused for every request until the
// user logs out or the app closes.
class NetworkClient : public QObject
{
    Q_OBJECT
public:
    static NetworkClient& instance();

    bool connectToServer(const QString &host = "127.0.0.1", quint16 port = 1234);
    void disconnectFromServer();
    bool isConnected() const;

    // Sends a request and blocks until the matching response arrives (or a
    // request times out). On a connection/timeout failure, returns a
    // {"status": "Failure", "message": ...} object rather than throwing.
    QJsonObject sendRequest(RequestType type, const QJsonObject &data = QJsonObject());

    QString lastError() const;

private:
    NetworkClient();
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;

    bool readFrame(QJsonObject &out);
    QJsonObject failureResponse(const QString &message);

    QTcpSocket socket;
    QString m_lastError;
};

#endif // NETWORKCLIENT_H
