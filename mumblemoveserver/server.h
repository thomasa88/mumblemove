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

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QSharedPointer>
#include <QtNetwork>

#include "connection.h"
#include "protocol.h"

class Server : public QObject {
    Q_OBJECT

public:
    Server();
    virtual ~Server() = default;

private:
    QTcpServer tcpServer;
    QMap<Connection *, QSharedPointer<Connection>> connections;
    quint64 nextId = 0;

public slots:
    void run();

private slots:
    void newConnection();
    void clientUpdate(const ClientInfo &clientInfo);
};

#endif // SERVER_H
