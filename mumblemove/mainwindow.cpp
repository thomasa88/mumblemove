#include "avatar.h"
#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QCursor>
#include <QDebug>
#include <QTimer>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("thomasa88", "MumbleMove")
{
    ui->setupUi(this);

    scene.setSceneRect(-100, -100, 200, 200);

    // Must use "new" as scene takes ownership of the item
    user_avatar = new Avatar();
    user_avatar->setPos(0, 0);
    scene.addItem(user_avatar);
    ui->graphicsView->setCursor(Qt::PointingHandCursor);
    ui->graphicsView->setScene(&scene);

    connect(&scene, &Scene::mouseClick, this, &MainWindow::sceneClick);

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
    user_avatar->setName(settings.value("name", "!").value<QString>());
    user_avatar->setColor(settings.value("color").value<QColor>());
}

void MainWindow::sceneClick(qreal x, qreal y)
{
    user_avatar->setPos(x, y);
}

int MainWindow::showSettings()
{
    SettingsDialog settingsDialog(this);
    settingsDialog.setServer(settings.value("server", "").value<QString>());
    settingsDialog.setName(settings.value("name", "").value<QString>());
    settingsDialog.setColor(settings.value("color", QColor(Qt::gray)).value<QColor>());
    int result = settingsDialog.exec();
    if (result == QDialog::Accepted) {
        settings.setValue("server", settingsDialog.getServer());
        settings.setValue("name", settingsDialog.getName());
        settings.setValue("color", settingsDialog.getColor());
    }
    return result;
}
