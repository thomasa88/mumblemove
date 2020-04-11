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
    QSharedPointer<Connection> connection(new Connection(clientSocket));
    connect(connection.get(), &Connection::clientUpdate, this, &Server::clientUpdate);
    connect(connection.get(), &Connection::disconnected, this, &Server::clientDisconnected);
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
        if (other_connection != connection) {
            other_connection->send(clientInfo);
        }
    }
    if (clientInfo.remove) {
        qWarning() << "CONTAINS" << connections.contains(connection);
        connections.remove(connection);
    }
}

void Server::clientDisconnected() {
    Connection *connection = qobject_cast<Connection *>(sender());
    if (connection == nullptr) {
        return;
    }
    ClientInfo clientInfo = connection->getClientInfo();
    qInfo() << "Client disconnected:" << clientInfo.name;
    clientInfo.remove = true;
    clientUpdate(clientInfo);
}
