#ifndef TESTOBJ_H
#define TESTOBJ_H

#include <QObject>

// Need one class implementing QObject for the MOC thing to work for other classes

class Testobj : public QObject
{
    Q_OBJECT
public:
    Testobj();
};

#endif // TESTOBJ_H
