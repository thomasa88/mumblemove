#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QSettings>

#include "avatar.h"
#include "client.h"
#include "mumblelink.h"
#include "scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    int showSettings();
    void applySettings();
    void connectToServer();

    Ui::MainWindow *ui;
    MumbleLink mumble_link;
    Scene scene;
    Avatar *user_avatar;
    QSettings settings;
    Client client;
    QMap<quint64, Avatar *> others;

signals:
    void connectClient(const QString &host);
    void disconnectClient();
    void updatePosition(const QPoint &position);

private slots:
    void windowLoaded();
    void sceneClick(QPointF position);
    void viewContextMenu(QContextMenuEvent *event);
    void moveWindow(int x, int y);

    void userUpdated(quint64 id, const QString &name, const QColor &color, const QPointF &position);
    void userRemoved(quint64 id);
    void connectionError(const QString &message);
    void disconnected();
    void connected();
};

#endif // MAINWINDOW_H
