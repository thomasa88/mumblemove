#include "protocol.h"

QDataStream &operator>>(QDataStream &stream, ClientInfo &info) {
    stream >> info.id;
    stream >> info.name;
    stream >> info.color;
    stream >> info.position;
    stream >> info.remove;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const ClientInfo &info) {
    stream << info.id;
    stream << info.name;
    stream << info.color;
    stream << info.position;
    stream << info.remove;
    return stream;
}
