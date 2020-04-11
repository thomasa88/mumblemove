#ifndef SCALEINDICATOR_H
#define SCALEINDICATOR_H

#include <QFont>
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>

class ScaleIndicator : public QGraphicsItem
{
public:
    ScaleIndicator(qreal pixelsPerMeter);
    ~ScaleIndicator() override = default;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    qreal pixelsPerMeter;
    QFont font;
};

#endif // SCALEINDICATOR_H
