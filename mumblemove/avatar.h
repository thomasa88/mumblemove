#ifndef AVATAR_H
#define AVATAR_H

#include <QGraphicsItem>
#include <QPainter>

class Avatar : public QGraphicsItem
{
public:
    Avatar();

    void setName(const QString &name);
    void setColor(const QColor &color);
    void setBorder(bool on);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

private:
    QColor color = Qt::lightGray;
    QString name;
    bool border = false;
};

#endif // AVATAR_H
