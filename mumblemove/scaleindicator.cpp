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

#include "scaleindicator.h"

#include <QGraphicsItem>
#include <QPainter>

ScaleIndicator::ScaleIndicator(qreal pixelsPerMeter) :
    pixelsPerMeter(pixelsPerMeter)
{
    font.setFamily("Helvetica");
    font.setPixelSize(static_cast<int>(boundingRect().height()));
}

QRectF ScaleIndicator::boundingRect() const {
    qreal width = pixelsPerMeter * 10.0 + 30.0;
    qreal height = 10.0;
    return QRectF(0, -height, width, height);
}

void ScaleIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    int indicatorHeight = 3;
    int indicatorWidth = static_cast<int>(10.0 * pixelsPerMeter);
    painter->setPen(QPen(Qt::gray));
    painter->drawLine(0, -indicatorHeight, 0, 0);
    painter->drawLine(0, -indicatorHeight / 2, indicatorWidth, -indicatorHeight / 2);
    painter->drawLine(indicatorWidth, 0, indicatorWidth, -indicatorHeight);

    int height = static_cast<int>(boundingRect().height());
    painter->setFont(font);
    painter->drawText(indicatorWidth + 2, -height + 1, 30, height, Qt::AlignLeft | Qt::AlignTop, "10 m");
}

