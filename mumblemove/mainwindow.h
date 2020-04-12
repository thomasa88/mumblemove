// MumbleMove
//
// Copyright (C) 2020  Thomas Axelsson
//
// This file is part of MumbleMove.
//
// MumbleMove is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MumbleMove is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MumbleMove.  If not, see <https://www.gnu.org/licenses/>.
//

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
    void updateMumble();

    Ui::MainWindow *ui;
    QSharedPointer<MumbleLink> mumbleLink;
    Scene scene;
    MainView* view;
    Avatar *userAvatar;
    QGraphicsSimpleTextItem *statusText;
    QSettings settings;
    Client client;
    QMap<quint64, Avatar *> others;
    QTimer flashResetTimer;
    QTimer mumbleUpdateTimer;

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
