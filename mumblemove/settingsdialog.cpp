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
    // Does not work in Windows
    //ui->colorButton->setPalette(value);

    // Works in Windows, but without border
//    QPalette palette = ui->colorButton->palette();
//    palette.setColor(QPalette::Button, value);
//    ui->colorButton->setFlat(true);
//    ui->colorButton->setAutoFillBackground(true);
//    ui->colorButton->setPalette(palette);

    ui->colorButton->setStyleSheet(QString("border: 1px solid black; background-color: %1").arg(value.name()));
}

void SettingsDialog::showColorDialog() {
    QColorDialog dialog;
    QColor selectedColor = dialog.getColor(getColor(), this);
    if (selectedColor.isValid()) {
        setColor(selectedColor);
    }
}
