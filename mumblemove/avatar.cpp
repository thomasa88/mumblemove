// MumbleMove
//
// Copyright (C) 2020  Thomas Axelsson
//
// This file is part of MumbleMove.
//
// MumbleMove is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MumbleMove is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MumbleMove.  If not, see <https://www.gnu.org/licenses/>.
//

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

const QString &Avatar::getName() const {
    return name;
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

