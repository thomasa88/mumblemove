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

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QCursor>
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
    MainView();
    virtual ~MainView() override = default;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool dragging = false;
    QPoint dragStart;
    QCursor defaultCursor = Qt::PointingHandCursor;

signals:
    void contextMenu(QContextMenuEvent *event);
    void moveWindow(int x, int y);
};

#endif // MAINVIEW_H
