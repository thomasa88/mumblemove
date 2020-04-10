#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QColorDialog>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->serverEdit->setFocus();

    connect(ui->colorButton, &QPushButton::clicked, this, &SettingsDialog::showColorDialog);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

QString SettingsDialog::getServer() const
{
    return ui->serverEdit->text();
}

void SettingsDialog::setServer(const QString &value) {
    ui->serverEdit->setText(value);
}

QString SettingsDialog::getName() const
{
    return ui->nameEdit->text();
}


void SettingsDialog::setName(const QString &value) {
    ui->nameEdit->setText(value);
}

QColor SettingsDialog::getColor() const
{
    return ui->colorButton->palette().color(QPalette::Button);
}

void SettingsDialog::setColor(const QColor &value) {
    ui->colorButton->setPalette(value);
}

void SettingsDialog::showColorDialog() {
    QColorDialog dialog;
    QColor selectedColor = dialog.getColor(getColor(), this);
    if (selectedColor.isValid()) {
        setColor(selectedColor);
    }
}
