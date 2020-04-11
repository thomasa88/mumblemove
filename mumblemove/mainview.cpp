#include "mainview.h"

#include <QDebug>
#include <QMouseEvent>

MainView::MainView() {
    setCursor(defaultCursor);
}

void MainView::contextMenuEvent(QContextMenuEvent *event) {
    emit contextMenu(event);
}

void MainView::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        emit moveWindow(event->globalX() - dragStart.x(), event->globalY() - dragStart.y());
    } else {
        // Propagate the event to the children
        QGraphicsView::mouseMoveEvent(event);
    }
}

void MainView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier) {
        dragStart = event->pos() + pos();
        dragging = true;
        setCursor(Qt::SizeHorCursor);
    } else {
        // Propagate the event to the children
        QGraphicsView::mousePressEvent(event);
    }
}

void MainView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        setCursor(defaultCursor);
    }

    // Propagate the event to the children
    QGraphicsView::mouseReleaseEvent(event);
}
