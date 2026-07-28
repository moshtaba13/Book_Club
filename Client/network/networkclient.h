#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QString>

#include "protocol.h"


class NetworkClient : public QObject
{
    Q_OBJECT
public:
    static NetworkClient& instance();

    bool connectToServer(const QString &host = "127.0.0.1", quint16 port = 1234);
    void disconnectFromServer();
    bool isConnected() const;


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
