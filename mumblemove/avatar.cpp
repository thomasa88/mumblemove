#include "avatar.h"

#include <QCursor>

Avatar::Avatar()
{
    //setCursor(Qt::OpenHandCursor);
}

void Avatar::setName(const QString &name)
{
    setToolTip(name);
    this->name = name;
    update();
}

void Avatar::setColor(const QColor &color) {
    this->color = color;
    update();
}

QRectF Avatar::boundingRect() const
{
    return QRectF(-15, -15, 30, 30);
}

void Avatar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::NoPen));
    painter->setBrush(QBrush(color));
    painter->drawEllipse(boundingRect());
    painter->setPen(QPen(Qt::black));
    painter->drawText(boundingRect(), Qt::AlignCenter, name.left(1));
}

