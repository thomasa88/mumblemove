#ifndef AVATAR_H
#define AVATAR_H

#include <QFont>
#include <QGraphicsItem>
#include <QPainter>

class Avatar : public QGraphicsItem
{
public:
    Avatar();

    void setName(const QString &name);
    void setColor(const QColor &color);
    void setBorder(bool on);
    qreal getSize() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

private:
    const qreal SIZE = 14.0;

    QColor color = Qt::lightGray;
    QString name;
    bool border = false;
    QFont font;
};

#endif // AVATAR_H
