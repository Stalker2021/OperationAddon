#include "settingwindow.h"
#include "ui_settingwindow.h"

SettingWindow::SettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);

    gameCounter = 0;
    allGame = 0;
    currentGame = 0;

    folderSelector.setFileMode(QFileDialog::Directory);
    folderSelector.setOption(QFileDialog::ShowDirsOnly);
    folderSelector.setModal(true);
}

SettingWindow::~SettingWindow()
{
    allGame = 0;
    currentGame = 0;

    delete ui;
}

void SettingWindow::on_pushButtonCancel_clicked()
{
    this->close();
}

void SettingWindow::on_pushButtonGame_clicked()
{
    if(folderSelector.exec())
        ui->lineEditGame->setText(folderSelector.directory().absolutePath());
}

void SettingWindow::on_pushButtonAddon_clicked()
{
    if(folderSelector.exec())
        ui->lineEditAddon->setText(folderSelector.directory().absolutePath());
}

void SettingWindow::on_pushButtonSave_clicked()
{
    currentGame->gameDir = ui->lineEditGame->text();
    currentGame->addonDir = ui->lineEditAddon->text();

    QFile settingFile(PATH_SETTING);
    if (settingFile.open(QIODevice::WriteOnly))
    {
       QTextStream writeFile(&settingFile);

       for(int iter = 0; iter < gameCounter; iter++)
       {
           writeFile << allGame[iter].gameDir + "\n";
           writeFile << allGame[iter].addonDir + "\n";
       }
       settingFile.close();
    }

    this->close();
}

//////////////// My Functions ////////////////

void SettingWindow::setGamesPtr(int num, GameClass* all, GameClass* current)
{
    gameCounter = num;
    allGame = all;
    currentGame = current;

    ui->lineEditGame->setText(currentGame->gameDir);
    ui->lineEditAddon->setText(currentGame->addonDir);
}
