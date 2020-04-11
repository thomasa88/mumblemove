#include "avatar.h"
#include "mainwindow.h"
#include "mainview.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QContextMenuEvent>
#include <QCursor>
#include <QDebug>
#include <QMenu>
#include <QTimer>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("thomasa88", "MumbleMove")
{
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    setWindowFlag(Qt::FramelessWindowHint, true);
    ui->setupUi(this);

    scene.setSceneRect(-100, -100, 200, 200);

    // Must use "new" as scene takes ownership of the item
    user_avatar = new Avatar();
    user_avatar->setPos(0, 0);
    user_avatar->setZValue(100.0);
    scene.addItem(user_avatar);

    MainView *view = new MainView();
    ui->centralWidget->layout()->addWidget(view);
    view->setCursor(Qt::PointingHandCursor);
    view->setScene(&scene);

    connect(&scene, &Scene::mouseClick, this, &MainWindow::sceneClick);
    connect(view, &MainView::contextMenu, this, &MainWindow::viewContextMenu);
    connect(view, &MainView::moveWindow, this, &MainWindow::moveWindow);

    connect(&client, &Client::gotPosition, this, &MainWindow::userUpdated);
    connect(&client, &Client::clientRemoved, this, &MainWindow::userRemoved);
    connect(&client, &Client::connectionError, this, &MainWindow::connectionError);
    connect(&client, &Client::disconnected, this, &MainWindow::disconnected);
    connect(&client, &Client::clientConnected, this, &MainWindow::connected);

//    connect(this, &MainWindow::updatePosition, &client, &Client::updatePosition);
//    connect(this, &MainWindow::connectClient, &client, &Client::connectClient);
//    connect(this, &MainWindow::disconnectClient, &client, &Client::disconnectClient);

    mumble_link.update();

    QTimer::singleShot(0, this, &MainWindow::windowLoaded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::windowLoaded()
{
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

void MainWindow::sceneClick(QPointF position)
{
    user_avatar->setPos(position);
    emit client.updatePosition(position);
}

void MainWindow::viewContextMenu(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction("Ctrl+Mouse to move window");
    menu.addSeparator();
    menu.addAction("&Settings...", this, &MainWindow::showSettings);
    menu.addAction("&Exit", &QApplication::quit);
    menu.exec(event->globalPos());
}

void MainWindow::moveWindow(int x, int y)
{
    move(x, y);
}

void MainWindow::userUpdated(quint64 id, const QString &name, const QColor &color, const QPointF &position) {
    Avatar *other = nullptr;
    if (!others.contains(id)) {
        other = new Avatar();
        others.insert(id, other);
        scene.addItem(other);
    } else {
        other = others[id];
    }
    other->setName(name);
    other->setColor(color);
    other->setPos(position);
}

void MainWindow::userRemoved(quint64 id) {
    qDebug() << "USER REMOVED";
    if (others.contains(id)) {
        scene.removeItem(others[id]);
        others.remove(id);
    }
}

void MainWindow::connectionError(const QString &message) {
    qDebug() << "Connection error:" << message;
}

void MainWindow::disconnected() {
    qDebug() << "Disconnected";
}

void MainWindow::connected() {
    qDebug() << "Connected";
}

int MainWindow::showSettings()
{
    SettingsDialog settingsDialog(this);
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
    user_avatar->setName(name);
    user_avatar->setColor(color);
    client.setInfo(name, color);
    connectToServer();
}
