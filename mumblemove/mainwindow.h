#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QSettings>
#include <QTimer>

#include "avatar.h"
#include "client.h"
#include "mumblelink.h"
#include "scene.h"
#include "mainview.h"

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
    void setStatus(const QString &message);
    void flash(const QColor &color);
    void flashReset();

    Ui::MainWindow *ui;
    MumbleLink mumbleLink;
    Scene scene;
    MainView* view;
    Avatar *userAvatar;
    QGraphicsSimpleTextItem *statusText;
    QSettings settings;
    Client client;
    QMap<quint64, Avatar *> others;
    QTimer flashResetTimer;

signals:
    void connectClient(const QString &host);
    void disconnectClient();
    void updatePosition(const QPoint &position);

private slots:
    void windowLoaded();
    void moveAvatar(QPointF position);
    void viewContextMenu(QContextMenuEvent *event);
    void moveWindow(int x, int y);

    void userUpdated(quint64 id, const QString &name, const QColor &color, const QPointF &position);
    void userRemoved(quint64 id);
    void connectionError(const QString &message);
    void disconnected();
    void connected();
};

#endif // MAINWINDOW_H
