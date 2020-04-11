#include "connection.h"
#include "protocol.h"

Connection::Connection(QAbstractSocket *socket) :
    socket(socket)
{
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_5_12);
    //out.setVersion(QDataStream::Qt_5_12);
    connect(socket, &QAbstractSocket::disconnected, this, &Connection::disconnected);
    connect(socket, &QAbstractSocket::connected, this, &Connection::connectedToHost);
    connect(socket, &QIODevice::readyRead, this, &Connection::data);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Connection::error);
}

const ClientInfo &Connection::getClientInfo() const
{
    return clientInfoBuf;
}

//void Connection::disconnected() {
//    qInfo() << "Client disconnected:" << clientInfoBuf.name;
//    clientInfoBuf.remove = true;
//    emit clientUpdate(clientInfoBuf);
//}

void Connection::data() {
    in.startTransaction();
    in >> clientInfoBuf;
    if (!in.commitTransaction()) {
        // Need to receive more
        return;
    }
    qInfo() << "Client update:" << clientInfoBuf.name << clientInfoBuf.position;
    emit clientUpdate(clientInfoBuf);
}

void Connection::error(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qDebug() << "Connection error:" << socket->errorString();
    emit connectionError(socket->errorString());
}

void Connection::send(const ClientInfo &clientInfo) {
    //out << clientInfo;
    in << clientInfo;
}
