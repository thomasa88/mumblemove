#include "avatar.h"
#include "mainwindow.h"
#include "mainview.h"
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("thomasa88", "MumbleMove")
{
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    setWindowFlag(Qt::FramelessWindowHint, true);
    ui->setupUi(this);

    QRect geo = QGuiApplication::primaryScreen()->geometry();
    geo.setX(geo.width() - 100 - 200);
    geo.setWidth(200);
    geo.setHeight(50);
    setGeometry(geo);

    view = new MainView();
    view->setCursor(Qt::PointingHandCursor);
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
    userAvatar->setPos(scene.sceneRect().width() / 2.0,
                       scene.sceneRect().height() / 2.0);
    userAvatar->setZValue(100.0);
    scene.addItem(userAvatar);

    statusText = new QGraphicsSimpleTextItem();
    statusText->setPos(2, 2);
    statusText->setBrush(Qt::gray);
    statusText->setZValue(200.0);
    scene.addItem(statusText);

    connect(&scene, &Scene::mouseClick, this, &MainWindow::sceneClick);
    connect(view, &MainView::contextMenu, this, &MainWindow::viewContextMenu);
    connect(view, &MainView::moveWindow, this, &MainWindow::moveWindow);

    connect(&client, &Client::gotPosition, this, &MainWindow::userUpdated);
    connect(&client, &Client::clientRemoved, this, &MainWindow::userRemoved);
    connect(&client, &Client::connectionError, this, &MainWindow::connectionError);
    connect(&client, &Client::disconnected, this, &MainWindow::disconnected);
    connect(&client, &Client::clientConnected, this, &MainWindow::connected);

    mumbleLink.update();

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

void MainWindow::sceneClick(QPointF position)
{
    userAvatar->setPos(position);
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
    move(x, 0);
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
    userAvatar->setName(name);
    userAvatar->setColor(color);
    client.setInfo(name, color);
    connectToServer();
}
