#include "server.h"

#include <QDebug>
#include <QSharedPointer>

#include "connection.h"
#include "protocol.h"

Server::Server() {
    connect(&tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::run() {
    qInfo() << "MumbleMove Server";
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
