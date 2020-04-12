// MumbleMove Server
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

    qInfo() << "Client" << clientId << "connected";
}

const ClientInfo &Connection::getClientInfo() const
{
    return clientInfo;
}

void Connection::disconnected() {
    qInfo() << "Client" << clientId << "disconnected:" << clientInfo.name;
    clientInfo.remove = true;
    emit clientUpdate(clientInfo);
}

void Connection::receive() {
    // We are only working with one client per connection,
    // so we can wait and just send the last message, the
    // old ones will be outdated
    bool gotData = false;
    QString oldName = clientInfo.name;
    ClientInfo buf;
    while (!dataStream.atEnd()) {
        dataStream.startTransaction();
        dataStream >> buf;
        if (!dataStream.commitTransaction()) {
            // Need to receive more
            break;
        }
        // Save the data before we try to receive more
        buf.id = clientId;
        clientInfo = buf;
        gotData = true;
    }
    if (gotData) {
        qDebug() << "Client" << clientId << "data";
        if (clientInfo.name != oldName) {
            qInfo() << "Client" << clientId << "name update:" << clientInfo.name;
        }
        emit clientUpdate(clientInfo);
    }
}

void Connection::error(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qDebug() << "Connection error:" << socket->errorString();
    emit connectionError(socket->errorString());
}

void Connection::send(const ClientInfo &clientInfo) {
    qDebug() << "Send to" << clientId;
    dataStream << clientInfo;
}
