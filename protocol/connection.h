#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtNetwork>
#include <QObject>
#include "protocol.h"

class Connection : public QObject
{
    Q_OBJECT

public:
    Connection(QAbstractSocket *socket);
    virtual ~Connection() = default;

    const ClientInfo &getClientInfo() const;
    void send(const ClientInfo &clientInfoBuf);

signals:
    void clientUpdate(const ClientInfo &clientInfoBuf);
    void connectionError(const QString &message);
    void connectedToHost();
    void disconnected();

public slots:


private slots:
    void data();
    void error(QAbstractSocket::SocketError socketError);

private:
    QSharedPointer<QAbstractSocket> socket;
    QDataStream in;
    //QDataStream out;
    ClientInfo clientInfoBuf;
};

#endif // CONNECTION_H
