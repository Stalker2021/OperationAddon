#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QComboBox>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QProcess>

#include "defines.h"
#include "mystruct.h"
#include "settingwindow.h"
#include "iotextwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButtonG1_clicked();

    void on_pushButtonG2_clicked();

    void on_pushButtonG3_clicked();

    void on_pushButtonSetting_clicked();

    void on_comboBoxType1_currentIndexChanged(const QString &arg1);

    void on_comboBoxType2_currentIndexChanged(const QString &arg1);

    void on_comboBoxType3_currentIndexChanged(const QString &arg1);

    void on_listWidgetAddons_clicked();

    void on_listWidgetUsing_itemSelectionChanged();

    void on_listWidgetAddons_itemSelectionChanged();

    void on_listWidgetDepend_itemSelectionChanged();

    void on_pushButtonStartGame_clicked();

    void on_pushButtonOpenView_clicked();

    void on_pushButtonOpenDirectory_clicked();

    void on_pushButtonAddAddon_clicked();

    void on_pushButtonDeleteUsed_clicked();

    void on_pushButtonAddDepend_clicked();

    void on_listWidgetUsing_doubleClicked(const QModelIndex &index);

    void on_listWidgetDepend_doubleClicked(const QModelIndex &index);

    void on_menuBar_plus100_triggered();

    void on_menuBar_plus50_triggered();

    void on_menuBar_plus25_triggered();

    void on_menuBar_minus50_triggered();

    void on_menuBar_minus25_triggered();

    void on_menuBar_minus12_5_triggered();

    void on_menuBar_inGame_triggered();

    void on_menuBar_duplication_triggered();

    void on_menuBar_used_triggered();

    void on_menuBar_gameDirectory_triggered();

    void on_menuBar_addonsDirectory_triggered();

    void on_menuBar_bugsFile_triggered();

    void on_menuBar_hotKeys_triggered();

    void on_menuBar_about_triggered();

    void on_pushButtonLastAddon_clicked();

    void on_listWidgetAddons_currentRowChanged(int currentRow);

    void on_menuBar_byFileName_triggered();

    void on_menuBar_byInformation_triggered();

    void on_menuBar_depending_triggered();

    void keyReleaseEvent(QKeyEvent *eventKey);

    //void resizeEvent (QResizeEvent * e);

    //void keyPressEvent(QKeyEvent *ev);

private:
    Ui::MainWindow *ui;

    GameClass* allGames;
    GameClass* currentGame;
    int gamesCounter;
    AddonRemember* lastAddon;

    bool loadConfig();
    void loadSettings();
    QString parseNextConfig(QTextStream* readFile);
    QString getSelectedAddonPath();
    QString getSelectedAddonPathFull();
    void setImage(QLabel* lable, QString path);
    void setTypes(QString dir);
    void fillTypes(QComboBox* cb, QString path);
    void fillAddons(QString path);
    void fillUsedAddons(QString pathList);
    void loadDescrip();
    void rebootAddonsType();
    void reloadGameInfo();
    QString getAddonDir(QString addonType="");
    QString getAddonName(QString addonPath);
    bool checkCopyAddon(QString addonPath);
    bool copyAddonFile(QString file, QString addonType, QString srcDir = "");
    void saveUsedList();
    void changeScale(double scale);
    void setScale(QWidget* obj, double scale);
    QString checkAddonsIn(QString srcPath, QString destPath);
    QString checkAddonsIn(QString srcPath);
    QString checkFileIn(QString fileName, QString destPath);
    QString checkInformation(QString info, QString destPath);
    QString checkDepending(QString addon, QString destPath);
    void setWaitingMode(bool isOn);
    bool openDirectory(QString path);
};

#endif // MAINWINDOW_H
