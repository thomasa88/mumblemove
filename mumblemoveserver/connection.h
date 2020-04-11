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
