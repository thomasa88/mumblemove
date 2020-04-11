#include "avatar.h"

#include <QCursor>

Avatar::Avatar()
{
    font.setFamily("Helvetica");
    font.setPixelSize(static_cast<int>(boundingRect().height() * 0.7));
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

void Avatar::setBorder(bool on) {
    border = on;
    update();
}

qreal Avatar::getSize() const
{
    return SIZE;
}

QRectF Avatar::boundingRect() const
{
    return QRectF(-SIZE / 2.0, -SIZE / 2.0, SIZE, SIZE);
}

void Avatar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!border) {
        painter->setPen(QPen(Qt::NoPen));
    }
    painter->setBrush(QBrush(color));
    painter->drawEllipse(boundingRect());
    painter->setPen(QPen(Qt::black));
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, name.left(1));
}

