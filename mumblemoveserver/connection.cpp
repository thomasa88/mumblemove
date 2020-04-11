#include "connection.h"
#include "protocol.h"

Connection::Connection(quint64 clientId, QAbstractSocket *socket) :
    socket(socket)
{
    this->clientId = clientId;
    dataStream.setDevice(socket);
    dataStream.setVersion(QDataStream::Qt_5_12);
    connect(socket, &QAbstractSocket::disconnected, this, &Connection::disconnected);
    connect(socket, &QIODevice::readyRead, this, &Connection::receive);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Connection::error);
}

const ClientInfo &Connection::getClientInfo() const
{
    return clientInfoBuf;
}

void Connection::disconnected() {
    qInfo() << "Client disconnected:" << clientInfoBuf.name;
    clientInfoBuf.remove = true;
    emit clientUpdate(clientInfoBuf);
}

void Connection::receive() {
    QString oldName = clientInfoBuf.name;
    dataStream.startTransaction();
    dataStream >> clientInfoBuf;
    clientInfoBuf.id = clientId;
    if (!dataStream.commitTransaction()) {
        // Need to receive more
        return;
    }
    if (clientInfoBuf.name != oldName) {
        qInfo() << "Client name update:" << clientInfoBuf.id << clientInfoBuf.name;
    }
    emit clientUpdate(clientInfoBuf);
}

void Connection::error(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qDebug() << "Connection error:" << socket->errorString();
    emit connectionError(socket->errorString());
}

void Connection::send(const ClientInfo &clientInfo) {
    dataStream << clientInfo;
}
