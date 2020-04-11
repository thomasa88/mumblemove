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
    qint64 next_id = 0;

public slots:
    void run();

private slots:
    void newConnection();
    void clientUpdate(const ClientInfo &clientInfo);
    void clientDisconnected();
};

#endif // SERVER_H
