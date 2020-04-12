// MumbleMove
//
// Copyright (C) 2020  Thomas Axelsson
//
// This file is part of MumbleMove.
//
// MumbleMove is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MumbleMove is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MumbleMove.  If not, see <https://www.gnu.org/licenses/>.
//

#include "client.h"
#include "protocol.h"

Client::Client(QObject *parent) : QObject(parent) {

}

void Client::setInfo(const QString &name, const QColor &color) {
    // ID is not used when sending
    info.id = 0;

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
    socket.reset(new QTcpSocket());
    socket->connectToHost(host, PORT);
    dataStream.setDevice(socket.get());
    dataStream.setVersion(QDataStream::Qt_5_12);
    connect(socket.get(), &QAbstractSocket::disconnected, this, &Client::disconnected);
    connect(socket.get(), &QAbstractSocket::connected, this, &Client::connected);
    connect(socket.get(), &QIODevice::readyRead, this, &Client::receive);
    connect(socket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::error);
}

void Client::disconnectClient() {
    // connection.disconnect / destructor -> socket.disconnectFromHost() -> emits disconnected signal
    stayConnected = false;
    otherClients.clear();
    send();
}

void Client::updatePosition(const QPointF &position) {
    info.position = position;
    send();
}

void Client::send() {
    qDebug() << "Send" << info.position;
    info.remove = !stayConnected;
    //if (connection.isConnected()) {
        dataStream << info;
    //} or check/ignore error and retry when connected again?
}

void Client::receive() {
    qDebug() << "Receive";
    ClientInfo otherClientInfo;
    while (!dataStream.atEnd()) {
        dataStream.startTransaction();
        dataStream >> otherClientInfo;
        if (!dataStream.commitTransaction()) {
            // Need to receive more
            return;
        }

        if (!stayConnected) {
            // Just flush the socket. Skip?
            //continue;
            return;
        }
        if (otherClientInfo.remove) {
            otherClients.remove(otherClientInfo.id);
            emit clientRemoved(otherClientInfo.id);
        } else {
            otherClients.insert(otherClientInfo.id, otherClientInfo);
            emit gotPosition(otherClientInfo.id,
                             otherClientInfo.name,
                             QColor::fromRgb(otherClientInfo.color),
                             otherClientInfo.position);
        }
    }
}

void Client::error(const QAbstractSocket::SocketError &socketError) {
    Q_UNUSED(socketError);
    otherClients.clear();
    QString infoMessage = socket->errorString();
    if (stayConnected) {
        //infoMessage += " Retrying in 2 seconds..";
        QTimer::singleShot(2000, this, &Client::startConnection);
    }
    emit connectionError(infoMessage);
}

void Client::connected() {
    send();
    emit clientConnected();
}

