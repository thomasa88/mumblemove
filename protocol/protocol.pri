# TODO: Fix this mess
SOURCES += \
    ../protocol/protocol.cpp \
    ../protocol/connection.cpp

HEADERS += \
    ../protocol/protocol.h \
    ../protocol/connection.h

INCLUDEPATH += ../protocol


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
