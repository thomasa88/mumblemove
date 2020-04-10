#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSettings>

#include "avatar.h"
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

    Ui::MainWindow *ui;
    MumbleLink mumble_link;
    Scene scene;
    Avatar *user_avatar;
    QSettings settings;

private slots:
    void windowLoaded();
    void sceneClick(qreal x, qreal y);
    void viewContextMenu(QContextMenuEvent *event);
    void moveWindow(int x, int y);
};

#endif // MAINWINDOW_H
