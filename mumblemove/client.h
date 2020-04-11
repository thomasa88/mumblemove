#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QObject>
#include <QSharedPointer>

#include "connection.h"

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

    bool stayConnected = false;
    QSharedPointer<Connection> connection;
    ClientInfo info;
    QMap<qint64, ClientInfo> otherClients;
    QString host;

signals:
    void gotPosition(qint64 id, const QString &name, const QColor &color);
    void clientRemoved(qint64 id);
    void error(const QString &message);
    void disconnected();
    void connected();

public slots:
    void connectClient(const QString &host);
    void disconnectClient();
    void sendPosition(const QPoint &position);

private slots:
    void connectionError(const QString &message);
    void connectionConnected();
};

#endif // CLIENT_H
