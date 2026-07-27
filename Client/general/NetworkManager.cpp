#include "NetworkManager.h"

NetworkManager& NetworkManager::instance()
{
    static NetworkManager inst;
    return inst;
}

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), lastSentType(RequestType::Login)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onErrorOccurred);
}

void NetworkManager::connectToServer(const QString &host, quint16 port)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        return;
    }
    socket->connectToHost(host, port);
}

bool NetworkManager::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkManager::sendRequest(RequestType type, const QJsonObject &data)
{
    if (!isConnected()) {
        emit connectionFailed("Not connected to server.");
        return;
    }

    lastSentType = type;

    QJsonObject message;
    message["type"] = requestTypeToString(type);
    message["data"] = data;

    QJsonDocument doc(message);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    // دقیقاً هم‌فرمت با سرور: ۴ بایت طول پیام (quint32) + خود JSON
    QByteArray lengthBytes;
    QDataStream stream(&lengthBytes, QIODevice::WriteOnly);
    stream << (quint32)jsonData.size();

    socket->write(lengthBytes);
    socket->write(jsonData);
    socket->flush();
}

void NetworkManager::onConnected()
{
    emit connected();
}

void NetworkManager::onDisconnected()
{
    emit disconnected();
}

void NetworkManager::onReadyRead()
{
    buffer.append(socket->readAll());

    while (buffer.size() >= 4) {
        quint32 messageLength;
        QDataStream stream(buffer.left(4));
        stream >> messageLength;

        if (buffer.size() < 4 + (int)messageLength) {
            break; // پیام هنوز کامل نرسیده، صبر کن
        }

        QByteArray jsonData = buffer.mid(4, messageLength);
        buffer.remove(0, 4 + messageLength);

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isObject()) {
            continue;
        }

        QJsonObject obj = doc.object();
        ResponseStatus status = stringToResponseStatus(obj.value("status").toString());
        QJsonObject data = obj.value("data").toObject();
        QString msg = obj.value("message").toString();

        emit responseReceived(lastSentType, status, data, msg);
    }
}

void NetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit connectionFailed(socket->errorString());
}