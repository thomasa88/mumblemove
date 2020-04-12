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

#include "server.h"

#include <QDebug>
#include <QSharedPointer>

#include "connection.h"
#include "protocol.h"

Server::Server() {
    connect(&tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::run() {
    qInfo() << "MumbleMove Server " VERSION;
    if (!tcpServer.listen(QHostAddress::Any, PORT)) {
        qWarning() << "Failed to listen on port" << PORT;
        qApp->exit(1);
        return;
    }

    qInfo() << "Listening on port" << PORT;
}

void Server::newConnection() {
    QTcpSocket *clientSocket = tcpServer.nextPendingConnection();
    QSharedPointer<Connection> connection(new Connection(nextId++, clientSocket), &QObject::deleteLater);
    connect(connection.get(), &Connection::clientUpdate, this, &Server::clientUpdate);
    // Inform of all existing clients
    for (auto &other_connection : connections) {
        connection->send(other_connection->getClientInfo());
    }
    connections.insert(connection.get(), connection);
}

void Server::clientUpdate(const ClientInfo &clientInfo) {
    Connection *connection = qobject_cast<Connection *>(sender());
    if (connection == nullptr) {
        return;
    }
    // Inform all other clients
    for (auto &other_connection : connections) {
        if (other_connection.get() != connection) {
            other_connection->send(clientInfo);
        }
    }
    if (clientInfo.remove) {
        connections.remove(connection);
    }
}
