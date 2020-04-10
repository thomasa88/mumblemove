#ifndef AVATAR_H
#define AVATAR_H

//#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QPainter>

class Avatar : public QGraphicsItem
{
public:
    Avatar();

    void setName(const QString &name);
    void setColor(const QColor &color);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

private:
    QColor color = Qt::lightGray;
    QString name;
};

#endif // AVATAR_H
