#include "client.h"
#include "protocol.h"

#include <QColor>

Client::Client(QObject *parent) : QObject(parent) {

}

void Client::setInfo(const QString &name, const QColor &color) {
    info.name = name;
    info.color = color.rgb();
    //connection->send(info);
}

void Client::connectClient(const QString &host) {
    stayConnected = true;
    this->host = host;
    startConnection();
}

void Client::startConnection() {
    if (!stayConnected) {
        return;
    }
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(host, PORT);
    connection.reset(new Connection(socket));
    connect(connection.get(), &Connection::clientUpdate, this, &Client::clientUpdate);
    connect(connection.get(), &Connection::connectionError, this, &Client::connectionError);
    connect(connection.get(), &Connection::connectedToHost, this, &Client::connectionConnected);
    connect(connection.get(), &Connection::disconnected, this, &Client::disconnected);
}

void Client::disconnectClient() {
    // connection.disconnect / destructor -> socket.disconnectFromHost() -> emits disconnected signal
    stayConnected = false;
    otherClients.clear();
    info.remove = stayConnected;
}

void Client::sendPosition(const QPoint &position) {
    info.remove = stayConnected;
    info.position = position;
    //if (connection.isConnected()) {
        connection->send(info);
    //} or check/ignore error and retry when connected again?
}

void Client::connectionError(const QString &message) {
    otherClients.clear();
    QString infoMessage = message;
    if (stayConnected) {
        infoMessage += " Retrying in 2 seconds..";
        QTimer::singleShot(2000, this, &Client::startConnection);
    }
    emit error(infoMessage);
}

void Client::connectionConnected() {
    connection->send(info);
    emit connected();
}

void Client::clientUpdate(const ClientInfo &clientInfo) {
    if (!stayConnected) {
        return;
    }
    if (clientInfo.remove) {
        otherClients.remove(clientInfo.id);
        emit clientRemoved(clientInfo.id);
    } else {
        otherClients.insert(clientInfo.id, clientInfo);
        emit gotPosition(clientInfo.id, clientInfo.name, QColor::fromRgb(clientInfo.color));
    }
}
