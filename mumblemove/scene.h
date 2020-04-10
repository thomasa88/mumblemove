#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene();
    virtual ~Scene() override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void mouseClick(qreal x, qreal y);
};

#endif // SCENE_H
