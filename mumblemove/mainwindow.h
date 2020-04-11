#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void windowLoaded();
    void sceneClick(qreal x, qreal y);
    void viewContextMenu(QContextMenuEvent *event);
    void moveWindow(int x, int y);

    void userUpdated(qint64 id, const QString &name, const QColor &color);
    void userRemoved(qint64 id);
    void connectionError(const QString &message);
    void disconnected();
    void connected();
};

#endif // MAINWINDOW_H
