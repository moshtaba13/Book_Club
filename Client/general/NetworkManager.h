#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include "protocol.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    static NetworkManager& instance();

    void connectToServer(const QString &host = "127.0.0.1", quint16 port = 1234);
    bool isConnected() const;
    void sendRequest(RequestType type, const QJsonObject &data);

signals:
    void connected();
    void disconnected();
    void connectionFailed(const QString &errorMessage);
    void responseReceived(RequestType lastRequestType, ResponseStatus status, const QJsonObject &data, const QString &message);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    explicit NetworkManager(QObject *parent = nullptr);
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    QTcpSocket *socket;
    QByteArray buffer;
    RequestType lastSentType; // آخرین درخواستی که فرستادیم، برای تطبیق با پاسخ
};

#endif // NETWORKMANAGER_H