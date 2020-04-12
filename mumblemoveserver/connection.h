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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtNetwork>
#include <QObject>
#include "protocol.h"

class Connection : public QObject
{
    Q_OBJECT

public:
    Connection(quint64 clientId, QAbstractSocket *socket);
    virtual ~Connection() = default;

    const ClientInfo &getClientInfo() const;
    void send(const ClientInfo &clientInfo);

signals:
    void clientUpdate(const ClientInfo &clientInfo);
    void connectionError(const QString &message);

public slots:


private slots:
    void receive();
    void error(QAbstractSocket::SocketError socketError);
    void disconnected();

private:
    QSharedPointer<QAbstractSocket> socket;
    QDataStream dataStream;
    ClientInfo clientInfo;
    quint64 clientId;
};

#endif // CONNECTION_H
