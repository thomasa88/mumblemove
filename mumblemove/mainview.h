#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QObject>

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

signals:
    void contextMenu(QContextMenuEvent *event);
};

#endif // MAINVIEW_H
