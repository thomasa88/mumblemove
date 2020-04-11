# TODO: Fix this mess
SOURCES += \
    ../protocol/protocol.cpp

HEADERS += \
    ../protocol/protocol.h

INCLUDEPATH += ../protocol


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
