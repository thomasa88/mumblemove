#include "scene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // We need to "move" the avatar before the user starts moving the mouse
    if (event->buttons() == Qt::LeftButton) {
        emit mouseClick(event->scenePos());
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        // It seems this event only triggers when the mouse is down, by default :)
        emit mouseClick(event->scenePos());
    }
}
