#include <QCoreApplication>
#include <QTimer>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    QTimer::singleShot(0, &server, &Server::run);

    return a.exec();
}
