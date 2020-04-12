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

#ifndef CLIENT_H
#define CLIENT_H

#include <QColor>
#include <QtNetwork>
#include <QObject>
#include <QSharedPointer>

#include "protocol.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    virtual ~Client() = default;

    void setInfo(const QString &name, const QColor &color);

private:
    void clientUpdate(const ClientInfo &clientInfo);
    void startConnection();
    void send();

    bool stayConnected = false;
    ClientInfo info;
    QMap<quint64, ClientInfo> otherClients;
    QString host;
    QDataStream dataStream;
    QSharedPointer<QTcpSocket> socket;

signals:
    void gotPosition(quint64 id, const QString &name, const QColor &color, const QPointF &position);
    void clientRemoved(quint64 id);
    void connectionError(const QString &message);
    void disconnected();
    void clientConnected();

public slots:
    void connectClient(const QString &host);
    void disconnectClient();
    void updatePosition(const QPointF &position);

private slots:
    void receive();
    void error(const QAbstractSocket::SocketError &socketError);
    void connected();
};

#endif // CLIENT_H
