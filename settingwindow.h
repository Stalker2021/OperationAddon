#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QTextStream>

#include "defines.h"
#include "mystruct.h"

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = 0);
    ~SettingWindow();

    void setGamesPtr(int count, GameClass* all, GameClass* current);

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonGame_clicked();

    void on_pushButtonAddon_clicked();

    void on_pushButtonSave_clicked();

private:
    Ui::SettingWindow *ui;

     QFileDialog folderSelector;
     int gameCounter;
     GameClass* allGame;
     GameClass* currentGame;
};

#endif // SETTINGWINDOW_H
