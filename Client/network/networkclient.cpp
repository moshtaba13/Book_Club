#include "networkclient.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>

namespace {
constexpr int kRequestTimeoutMs = 5000;
}

NetworkClient::NetworkClient() : socket(this)
{
}

NetworkClient& NetworkClient::instance()
{
    static NetworkClient instance;
    return instance;
}

bool NetworkClient::connectToServer(const QString &host, quint16 port)
{
    if (socket.state() == QAbstractSocket::ConnectedState)
        return true;

    socket.abort();
    socket.connectToHost(host, port);

    if (!socket.waitForConnected(kRequestTimeoutMs)) {
        m_lastError = socket.errorString();
        return false;
    }

    return true;
}

void NetworkClient::disconnectFromServer()
{
    if (socket.state() != QAbstractSocket::UnconnectedState) {
        socket.disconnectFromHost();
        if (socket.state() != QAbstractSocket::UnconnectedState)
            socket.waitForDisconnected(1000);
    }
}

bool NetworkClient::isConnected() const
{
    return socket.state() == QAbstractSocket::ConnectedState;
}

QString NetworkClient::lastError() const
{
    return m_lastError;
}

QJsonObject NetworkClient::failureResponse(const QString &message)
{
    m_lastError = message;

    QJsonObject response;
    response["status"] = "Failure";
    response["message"] = message;
    return response;
}

QJsonObject NetworkClient::sendRequest(RequestType type, const QJsonObject &data)
{
    if (!isConnected() && !connectToServer())
        return failureResponse("Could not connect to the server: " + m_lastError);

    QJsonObject request;
    request["type"] = requestTypeToString(type);
    request["data"] = data;

    QByteArray payload = QJsonDocument(request).toJson(QJsonDocument::Compact);

    QByteArray header;
    QDataStream headerStream(&header, QIODevice::WriteOnly);
    headerStream << static_cast<quint32>(payload.size());

    socket.write(header);
    socket.write(payload);

    if (!socket.waitForBytesWritten(kRequestTimeoutMs))
        return failureResponse("Timed out sending request to the server.");

    QJsonObject response;
    if (!readFrame(response))
        return failureResponse(m_lastError.isEmpty() ? "No response from the server." : m_lastError);

    return response;
}

bool NetworkClient::readFrame(QJsonObject &out)
{
    QByteArray buffer;

    auto readAtLeast = [this, &buffer](int needed) -> bool {
        while (buffer.size() < needed) {
            if (socket.bytesAvailable() <= 0 && !socket.waitForReadyRead(kRequestTimeoutMs)) {
                m_lastError = "Timed out waiting for a response from the server.";
                return false;
            }
            buffer.append(socket.readAll());
        }
        return true;
    };

    if (!readAtLeast(4))
        return false;

    quint32 length = 0;
    QDataStream lengthStream(buffer.left(4));
    lengthStream >> length;
    buffer.remove(0, 4);

    if (!readAtLeast(static_cast<int>(length)))
        return false;

    QByteArray jsonPayload = buffer.left(length);

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonPayload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        m_lastError = "Received an invalid response from the server.";
        return false;
    }

    out = doc.object();
    return true;
}
