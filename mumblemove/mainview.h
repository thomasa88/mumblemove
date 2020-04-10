#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <QPoint>

//namespace Ui {
//class MainView;
//}

class MainView : public QGraphicsView
{
    Q_OBJECT

public:
    MainView() = default;
    virtual ~MainView() override = default;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool dragging = false;
    QPoint dragStart;

signals:
    void contextMenu(QContextMenuEvent *event);
    void moveWindow(int x, int y);
};

#endif // MAINVIEW_H
