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
    qreal width = pixelsPerMeter + 20.0;
    qreal height = 10.0;
    return QRectF(0, -height, width, height);
}

void ScaleIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    int indicatorHeight = 3;
    int indicatorWidth = static_cast<int>(1 * pixelsPerMeter);
    painter->setPen(QPen(Qt::gray));
    painter->drawLine(0, -indicatorHeight, 0, 0);
    painter->drawLine(0, -indicatorHeight / 2, indicatorWidth, -indicatorHeight / 2);
    painter->drawLine(indicatorWidth, 0, indicatorWidth, -indicatorHeight);

    int height = static_cast<int>(boundingRect().height());
    painter->setFont(font);
    painter->drawText(indicatorWidth + 2, -height + 1, 10, height, Qt::AlignLeft | Qt::AlignTop, "m");
}

