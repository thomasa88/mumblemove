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
