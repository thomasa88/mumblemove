#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QColor>
#include <QDialog>
#include <QString>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    QString getServer() const;
    void setServer(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    QColor getColor() const;
    void setColor(const QColor &value);

private slots:
    void showColorDialog();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
