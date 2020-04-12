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

#include "avatar.h"
#include "mainwindow.h"
#include "mainview.h"
#include "scaleindicator.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QContextMenuEvent>
#include <QCursor>
#include <QDebug>
#include <QDesktopWidget>
#include <QMenu>
#include <QScreen>
#include <QTimer>
#include <QVariant>

// TODO: Work the whole scene in meters?
const qreal PIXELS_PER_METER = 5.0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("thomasa88", "MumbleMove")
{
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    setWindowFlag(Qt::FramelessWindowHint, true);
    ui->setupUi(this);

    QRect geo = QGuiApplication::primaryScreen()->geometry();
    int xPos = settings.value("windowPosition", geo.width() - 100 - 200).toInt();
    geo.setX(xPos);
    geo.setWidth(200);
    geo.setHeight(50);
    setGeometry(geo);

    view = new MainView();
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view->setFrameStyle(QFrame::NoFrame);
    view->setFixedSize(geometry().size());
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->centralWidget->layout()->addWidget(view);

    scene.setSceneRect(0, 0, view->frameSize().width(), view->frameSize().height());
//    view->scale(view->frameSize().width() / scene.sceneRect().width(),
//                view->frameSize().height() / scene.sceneRect().height());
    view->setScene(&scene);
    //view->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

    // Must use "new" as scene takes ownership of the item
    userAvatar = new Avatar();
    userAvatar->setZValue(100.0);
    userAvatar->setBorder(true);
    moveAvatar(QPointF(scene.sceneRect().width() / 2.0, scene.sceneRect().height() / 2.0));
    scene.addItem(userAvatar);

    statusText = new QGraphicsSimpleTextItem();
    statusText->setPos(2, 2);
    statusText->setBrush(Qt::gray);
    statusText->setZValue(200.0);
    scene.addItem(statusText);
    setStatus("Disconnected");

    ScaleIndicator *scaleIndicator = new ScaleIndicator(PIXELS_PER_METER);
    scaleIndicator->setPos(2, scene.sceneRect().height() - 3.0);
    scene.addItem(scaleIndicator);

    connect(&scene, &Scene::mouseClick, this, &MainWindow::moveAvatar);
    connect(view, &MainView::contextMenu, this, &MainWindow::viewContextMenu);
    connect(view, &MainView::moveWindow, this, &MainWindow::moveWindow);

    connect(&client, &Client::gotPosition, this, &MainWindow::userUpdated);
    connect(&client, &Client::clientRemoved, this, &MainWindow::userRemoved);
    connect(&client, &Client::connectionError, this, &MainWindow::connectionError);
    connect(&client, &Client::disconnected, this, &MainWindow::disconnected);
    connect(&client, &Client::clientConnected, this, &MainWindow::connected);

    flashResetTimer.setSingleShot(true);
    flashResetTimer.setInterval(500);
    connect(&flashResetTimer, &QTimer::timeout, this, &MainWindow::flashReset);

    // If Mumble does not get data in a while, it declares the link to be lost
    // Don't run until settings has been applied once, to avoid "" user
    mumbleUpdateTimer.setInterval(1000);
    connect(&mumbleUpdateTimer, &QTimer::timeout, this, &MainWindow::updateMumble);

    QTimer::singleShot(0, this, &MainWindow::windowLoaded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::windowLoaded()
{
    //scene.setSceneRect(0, 0, view->frameSize().width(), view->frameSize().height());
//    view->scale(view->frameSize().width() / scene.sceneRect().width(),
//                view->frameSize().height() / scene.sceneRect().height());
    //view->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
    if (settings.value("server").isNull() ||
            settings.value("server").toString().isEmpty()) {
        int result = showSettings();
        if (result != QDialog::Accepted) {
           QApplication::exit();
        }
    }
    applySettings();
}

void MainWindow::connectToServer() {
    emit client.connectClient(settings.value("server").toString());
}

void MainWindow::setStatus(const QString &message)
{
    statusText->setText(message);
    statusText->setToolTip(message);
}

void MainWindow::flash(const QColor &color) {
    view->setBackgroundBrush(color);
    flashResetTimer.start();
}

void MainWindow::flashReset() {
    view->setBackgroundBrush(Qt::white);
}

void MainWindow::moveAvatar(QPointF position)
{
    userAvatar->setPos(position);
    updateMumble();
    emit client.updatePosition(position);
}

void MainWindow::viewContextMenu(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction("MumbleMove " VERSION);
    menu.addAction("Ctrl+Mouse to move window");
    menu.addSeparator();
    menu.addAction("&Settings...", this, &MainWindow::showSettings);
    menu.addAction("&Exit", &QApplication::quit);
    menu.exec(event->globalPos());
}

void MainWindow::moveWindow(int x, int y)
{
    Q_UNUSED(y);
    move(x, 0);
    settings.setValue("windowPosition", x);
}

void MainWindow::userUpdated(quint64 id, const QString &name, const QColor &color, const QPointF &position) {
    Avatar *other = nullptr;
    QPointF otherOldPos;
    if (!others.contains(id)) {
        other = new Avatar();
        other->setBorder(true);
        others.insert(id, other);
        scene.addItem(other);
        otherOldPos = QPointF(-10000.0, -10000.0);
    } else {
        other = others[id];
        otherOldPos = other->pos();
    }

    other->setName(name);
    other->setColor(color);
    other->setPos(position);

    qreal closeLimit = userAvatar->getSize() * 2.0;
    bool wasClose = (otherOldPos - userAvatar->pos()).manhattanLength() < closeLimit;
    bool isClose = (other->pos() - userAvatar->pos()).manhattanLength() < closeLimit;
    if (isClose && !wasClose) {
        flash(color);
    }
}

void MainWindow::userRemoved(quint64 id) {
    if (others.contains(id)) {
        scene.removeItem(others[id]);
        others.remove(id);
    }
}

void MainWindow::connectionError(const QString &message) {
    qDebug() << "Connection error:" << message;
    setStatus(tr("Disconnected (%1)").arg(message));
}

void MainWindow::disconnected() {
    qDebug() << "Disconnected";
    // connectionError comes first, and gives more info
    //setStatus("Disconnected");
}

void MainWindow::connected() {
    qDebug() << "Connected";
    setStatus("");
}

int MainWindow::showSettings()
{
    // Don't set "this" as parent, as the dialog can move the main window in that case
    SettingsDialog settingsDialog;
    settingsDialog.setServer(settings.value("server", "").value<QString>());
    settingsDialog.setName(settings.value("name", "").value<QString>());
    settingsDialog.setColor(settings.value("color", QColor(Qt::gray)).value<QColor>());
    // This modal dialog will likely mess with our network communication
    int result = settingsDialog.exec();
    if (result == QDialog::Accepted) {
        settings.setValue("server", settingsDialog.getServer());
        settings.setValue("name", settingsDialog.getName());
        settings.setValue("color", settingsDialog.getColor());
        applySettings();
    }
    return result;
}

void MainWindow::applySettings() {
    QString name = settings.value("name", "!").value<QString>();
    QColor color = settings.value("color").value<QColor>();
    userAvatar->setName(name);
    userAvatar->setColor(color);
    client.setInfo(name, color);
    updateMumble();
    mumbleUpdateTimer.start();
    connectToServer();
}

void MainWindow::updateMumble() {
    if (mumbleLink.isNull()) {
        mumbleLink.reset(new MumbleLink);
    }
    // Position 0.0 is special in Mumble, it makes Mumble ignore the positional audio
    QPointF meterPos(userAvatar->pos().x() / PIXELS_PER_METER + 10000.0,
                     userAvatar->pos().y() / PIXELS_PER_METER + 10000.0);
    mumbleLink->update(userAvatar->getName(), meterPos);
}
