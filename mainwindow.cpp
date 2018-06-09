#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lastAddon = new AddonRemember;

    this->setFixedSize(this->size());

    if(!loadConfig())
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_MISSINGCONFIG, QMessageBox::Ok);
        errorMessage.exec();
        this->setEnabled(false);
        return;
    }
    currentGame = allGames;

    loadSettings();

    ui->pushButtonG1->setVisible(false);
    ui->pushButtonG2->setVisible(false);
    ui->pushButtonG3->setVisible(false);

    switch(gamesCounter)
    {
        case 3:
            ui->pushButtonG3->setText(allGames[CODE_OF_G3].name);
            ui->pushButtonG3->setVisible(true);
        case 2:
            ui->pushButtonG2->setText(allGames[CODE_OF_G2].name);
            ui->pushButtonG2->setVisible(true);
        case 1:
            ui->pushButtonG1->setText(allGames[CODE_OF_G1].name);
            ui->pushButtonG1->setVisible(true);
        case 0:
            break;
        default:
            QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_TOOMANYGAMES, QMessageBox::Ok);
            errorMessage.exec();
            this->setEnabled(false);
            return;
    }

    ui->comboBoxType2->setVisible(false);
    ui->comboBoxType3->setVisible(false);

    ui->pushButtonOpenView->setVisible(false);
    ui->pushButtonOpenDirectory->setVisible(false);

    if(currentGame)
        reloadGameInfo();
}

MainWindow::~MainWindow()
{
    currentGame = 0;

    delete [] allGames;
    allGames = 0;

    delete lastAddon;
    lastAddon = 0;

    delete ui;
}

void MainWindow::on_pushButtonG1_clicked()
{
    ui->pushButtonG1->setEnabled(false);
    ui->pushButtonG2->setEnabled(true);
    ui->pushButtonG3->setEnabled(true);

    currentGame = allGames + CODE_OF_G1;

    reloadGameInfo();
}

void MainWindow::on_pushButtonG2_clicked()
{
    ui->pushButtonG1->setEnabled(true);
    ui->pushButtonG2->setEnabled(false);
    ui->pushButtonG3->setEnabled(true);

    currentGame = allGames + CODE_OF_G2;

    reloadGameInfo();
}

void MainWindow::on_pushButtonG3_clicked()
{
    ui->pushButtonG1->setEnabled(true);
    ui->pushButtonG2->setEnabled(true);
    ui->pushButtonG3->setEnabled(false);

    currentGame = allGames + CODE_OF_G3;

    reloadGameInfo();
}

void MainWindow::on_pushButtonSetting_clicked()
{
    SettingWindow settingWind;
    settingWind.setWindowTitle(WIND_NAME_SETTING + currentGame->name + "\"");
    settingWind.setWindowFlags(Qt::WindowCloseButtonHint);
    settingWind.setWindowIcon(QIcon(PATH_ICO_SETTING));
    settingWind.setGamesPtr(gamesCounter, allGames, currentGame);
    settingWind.exec();

    reloadGameInfo();
}

void MainWindow::on_pushButtonStartGame_clicked()
{
    QString buff;
    for(int iter = 0; ; iter++)
    {
        if(iter >= currentGame->pathExe.length())
        {
            QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_MISSINGEXE, QMessageBox::Ok);
            errorMessage.exec();
            return;
        }

        buff = "\"" + currentGame->gameDir + currentGame->pathExe[iter] + "\"";
        if(!system(qPrintable(buff)))
            break;
    }
}

void MainWindow::on_pushButtonOpenView_clicked()
{
    QString buff = getSelectedAddonPathFull() + NAME_FILE_PIC_PNG;

    if(!QFileInfo::exists(buff))
        buff = getSelectedAddonPathFull() + NAME_FILE_PIC_JPG;

    buff = "\"" + buff + "\"";
    system(qPrintable(buff));
}

void MainWindow::on_pushButtonOpenDirectory_clicked()
{
    openDirectory(getSelectedAddonPathFull());
}

void MainWindow::on_pushButtonAddAddon_clicked()
{
    if(!checkCopyAddon(getSelectedAddonPath()))
        return;

    QString addonType = getAddonDir();
    QDir dirAddon(getSelectedAddonPathFull());
    dirAddon.setNameFilters(QStringList(EXTENSION_ADDON));

    bool check = false;
    for(auto iter : dirAddon.entryList())
    {
        check = copyAddonFile(iter, addonType);
        if(!check)
            break;
    }

    if(check)
    {
        ui->listWidgetUsing->addItem(getSelectedAddonPath());
        ui->listWidgetUsing->sortItems();
        saveUsedList();
    }
}

void MainWindow::on_pushButtonDeleteUsed_clicked()
{
    QDir addonDir;
    addonDir.setNameFilters(QStringList(EXTENSION_ADDON));

    bool check;
    QString buff;
    for(auto iter : ui->listWidgetUsing->selectedItems())
    {
        addonDir.setPath(currentGame->addonDir + iter->text());
        if(addonDir.exists())
        {
            for(auto jter : addonDir.entryList())
            {
                buff = currentGame->gameDir + getAddonDir(iter->text().section('/', 1, 1)) + "/" + jter;
                if(QFileInfo::exists(buff))
                {
                    check = QFile::remove(buff);
                    if(!check)
                    {
                        QString addName = getAddonName(iter->text());
                        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_CANNOTDELETE + addName + "/" + jter + TEXT_MBREADONLY, QMessageBox::Ok);
                        errorMessage.exec();
                    }
                }
            }
        }
        else
        {
            QMessageBox errorMessage(ICON_QUESTION, TITLE_ATTENTION, TEXT_UNKNOWNADDON + iter->text() + TEXT_DELETEFROMUSED, QMessageBox::Yes | QMessageBox::No);
            if (errorMessage.exec() == QMessageBox::No)
                continue;
        }

        ui->listWidgetUsing->takeItem(ui->listWidgetUsing->row(iter));
    }

    saveUsedList();
}

void MainWindow::on_pushButtonAddDepend_clicked()
{
    QString addonType;
    QDir dirAddon;
    bool check;

    for(auto iter : ui->listWidgetDepend->selectedItems())
    {
        if(!checkCopyAddon(iter->text()))
            continue;

        addonType = getAddonDir(iter->text().section('/', 1, 1));
        dirAddon.setPath(currentGame->addonDir + iter->text());
        dirAddon.setNameFilters(QStringList(EXTENSION_ADDON));

        check = false;
        for(auto jter : dirAddon.entryList())
        {
            check = copyAddonFile(jter, addonType, currentGame->addonDir + iter->text());
            if(!check)
                break;
        }

        if(check)
        {
            ui->listWidgetUsing->addItem(iter->text());
            ui->listWidgetUsing->sortItems();
        }
    }

    saveUsedList();
}

void MainWindow::on_comboBoxType1_currentIndexChanged(const QString &arg1)
{
    ui->comboBoxType2->setVisible(false);
    ui->comboBoxType3->setVisible(false);

    if(ui->comboBoxType1->currentIndex() < 0)
    {
        ui->listWidgetAddons->clear();
        fillAddons(currentGame->addonDir);
        return;
    }

    fillTypes(ui->comboBoxType2, currentGame->addonDir + "/" + arg1);

    ui->listWidgetAddons->clear();
    ui->listWidgetDepend->clear();
    ui->textBrowserDescrip->clear();

    fillAddons(currentGame->addonDir + "/" + arg1);

    setImage(ui->labelImagePreview, PATH_APP + currentGame->logoFile);
    ui->pushButtonOpenView->setVisible(false);
    ui->pushButtonOpenDirectory->setVisible(false);

}

void MainWindow::on_comboBoxType2_currentIndexChanged(const QString &arg1)
{
    ui->comboBoxType3->setVisible(false);

    if(ui->comboBoxType2->currentIndex() < 0)
        return;

    fillTypes(ui->comboBoxType3, currentGame->addonDir + "/" + ui->comboBoxType1->currentText() + "/" + arg1);

    ui->listWidgetAddons->clear();
    ui->listWidgetDepend->clear();
    ui->textBrowserDescrip->clear();

    fillAddons(currentGame->addonDir + "/" + ui->comboBoxType1->currentText() + "/" + arg1);

    setImage(ui->labelImagePreview, PATH_APP + currentGame->logoFile);
    ui->pushButtonOpenView->setVisible(false);
    ui->pushButtonOpenDirectory->setVisible(false);
}

void MainWindow::on_comboBoxType3_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBoxType3->currentIndex() < 0)
        return;

    ui->listWidgetAddons->clear();
    ui->listWidgetDepend->clear();
    ui->textBrowserDescrip->clear();

    fillAddons(currentGame->addonDir + "/" + ui->comboBoxType1->currentText() + "/" + ui->comboBoxType2->currentText() + "/" + arg1);

    setImage(ui->labelImagePreview, PATH_APP + currentGame->logoFile);
    ui->pushButtonOpenView->setVisible(false);
    ui->pushButtonOpenDirectory->setVisible(false);
}

void MainWindow::on_listWidgetAddons_clicked()
{
    ui->listWidgetDepend->clear();
    ui->textBrowserDescrip->clear();

    QString addonPath = getSelectedAddonPathFull();
    QString buff = addonPath + NAME_FILE_PIC_PNG;
    bool check = true;

    if(!QFileInfo::exists(buff))
    {
        buff = addonPath + NAME_FILE_PIC_JPG;
        if(!QFileInfo::exists(buff))
        {
            buff = PATH_APP + currentGame->logoFile;
            check = false;
        }
    }
    setImage(ui->labelImagePreview, buff);
    ui->pushButtonOpenView->setVisible(check);

    ui->pushButtonOpenDirectory->setVisible(true);

    if(QFileInfo::exists(addonPath + NAME_FILE_DEPEND))
    {
        QFile dependFile(addonPath + NAME_FILE_DEPEND);
        if (dependFile.open(QIODevice::ReadOnly))
        {
           QTextStream readFile(&dependFile);
           while(!readFile.atEnd())
               ui->listWidgetDepend->addItem(readFile.readLine());

           dependFile.close();
        }

        ui->listWidgetDepend->sortItems();
    }

    loadDescrip();

}

void MainWindow::on_listWidgetUsing_itemSelectionChanged()
{
    ui->pushButtonDeleteUsed->setEnabled(ui->listWidgetUsing->selectedItems().length());
}

void MainWindow::on_listWidgetAddons_itemSelectionChanged()
{
    ui->pushButtonAddAddon->setEnabled(ui->listWidgetAddons->selectedItems().length());
}

void MainWindow::on_listWidgetDepend_itemSelectionChanged()
{
    ui->pushButtonAddDepend->setEnabled(ui->listWidgetDepend->selectedItems().length());

    for(int iter = 0; iter < ui->listWidgetUsing->count(); iter++)
    {
        if(ui->listWidgetDepend->currentItem()->text() == ui->listWidgetUsing->item(iter)->text())
        {
            ui->listWidgetUsing->item(iter)->setSelected(ui->listWidgetDepend->currentItem()->isSelected());
            break;
        }
    }
}

void MainWindow::on_listWidgetUsing_doubleClicked(const QModelIndex &index)
{
    if(!QFileInfo::exists(currentGame->addonDir + index.data().toString()))
    {
        QMessageBox errorMessage(ICON_QUESTION, TITLE_ATTENTION, TEXT_UNKNOWNADDON + index.data().toString() + TEXT_DELETEFROMUSED, QMessageBox::Yes | QMessageBox::No);
        if(errorMessage.exec() == QMessageBox::Yes)
            ui->listWidgetUsing->takeItem(index.row());

        return;
    }

    setTypes(index.data().toString());

}

void MainWindow::on_listWidgetDepend_doubleClicked(const QModelIndex &index)
{
    if(!QFileInfo::exists(currentGame->addonDir + index.data().toString()))
    {
        QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_UNKNOWNADDON + index.data().toString(), QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    setTypes(index.data().toString());
}

void MainWindow::on_menuBar_plus100_triggered()
{
    changeScale(2);
}

void MainWindow::on_menuBar_plus50_triggered()
{
    changeScale(1.5);
}

void MainWindow::on_menuBar_plus25_triggered()
{
    changeScale(1.25);
}

void MainWindow::on_menuBar_minus50_triggered()
{
    changeScale(1.0 / 2.0);
}

void MainWindow::on_menuBar_minus25_triggered()
{
    changeScale(1.0 / 1.5);
}

void MainWindow::on_menuBar_minus12_5_triggered()
{
    changeScale(1.0 / 1.25);
}

void MainWindow::on_menuBar_inGame_triggered()
{
    if(!QFileInfo::exists(currentGame->addonDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_ADDONDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    if(!QFileInfo::exists(currentGame->gameDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_GAMEDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    setWaitingMode(true);

    QString result;
    result = checkAddonsIn(currentGame->addonDir, currentGame->gameDir + KEY_ADDON_DIR);
    result += checkAddonsIn(currentGame->addonDir + "/" + KEY_CAMPAING, currentGame->gameDir + KEY_CAMPAING_DIR);
    result += checkAddonsIn(currentGame->addonDir + "/" + KEY_MISSION, currentGame->gameDir + KEY_MISSION_DIR);

    setWaitingMode(false);

    if(result.isEmpty())
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, TEXT_SAMENOTFOUND, QMessageBox::Ok);
        errorMessage.exec();
    }
    else
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, TEXT_SAMEFOUND + result, QMessageBox::Ok);
        errorMessage.exec();
    }
}

void MainWindow::on_menuBar_duplication_triggered()
{
    if(!QFileInfo::exists(currentGame->addonDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_ADDONDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    setWaitingMode(true);

    QString result;
    result = checkAddonsIn(currentGame->addonDir, currentGame->addonDir);

    setWaitingMode(false);

    if(result.isEmpty())
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, TEXT_SAMENOTFOUND, QMessageBox::Ok);
        errorMessage.exec();
    }
    else
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, TEXT_SAMEFOUND + result, QMessageBox::Ok);
        errorMessage.exec();
    }
}

void MainWindow::on_menuBar_used_triggered()
{
    if(!ui->listWidgetUsing->count())
    {
        QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_USEDEMPTY, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    if(!QFileInfo::exists(currentGame->addonDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_ADDONDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    setWaitingMode(true);

    QString result = "";
    QString check;
    for(int iter = 0; iter < ui->listWidgetUsing->count(); iter++)
    {
        check = checkAddonsIn(ui->listWidgetUsing->item(iter)->text());

        if(!check.isEmpty())
            result += check + "\n";
    }

    setWaitingMode(false);

    if(result.isEmpty())
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, TEXT_ALLUSEDNORM, QMessageBox::Ok);
        errorMessage.exec();
    }
    else
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, result, QMessageBox::Ok);
        errorMessage.exec();
    }
}

void MainWindow::on_menuBar_gameDirectory_triggered()
{
    if(!openDirectory(currentGame->gameDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_GAMEDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
    }
}

void MainWindow::on_menuBar_addonsDirectory_triggered()
{
    if(!openDirectory(currentGame->addonDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_ADDONDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
    }
}

void MainWindow::on_pushButtonLastAddon_clicked()
{
    if(lastAddon->last.isEmpty() || !QFileInfo::exists(currentGame->addonDir + lastAddon->last))
        return;

    setTypes(lastAddon->last);
}

void MainWindow::on_listWidgetAddons_currentRowChanged(int currentRow)
{
    if(currentRow < 0 && lastAddon->current.isEmpty())
        return;

    if(!lastAddon->current.isEmpty())
        lastAddon->last = lastAddon->current;

    if(currentRow < 0)
        lastAddon->current = "";
    else
        lastAddon->current = getSelectedAddonPath();

    if(!ui->pushButtonLastAddon->isEnabled())
        ui->pushButtonLastAddon->setEnabled(true);
}

void MainWindow::on_menuBar_byFileName_triggered()
{
    if(!QFileInfo::exists(currentGame->addonDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_ADDONDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    IOTextWindow ioTextWind;
    ioTextWind.setWindowTitle(WIND_NAME_FIND_FILE);
    ioTextWind.setWindowFlags(Qt::WindowCloseButtonHint);
    ioTextWind.setWindowIcon(QIcon(PATH_ICO_CHECKING));
    ioTextWind.exec();

    QString result = ioTextWind.getResult();
    if(result.isEmpty())
       return;

    setWaitingMode(true);

    result = checkFileIn(result, currentGame->addonDir);

    setWaitingMode(false);

    if(result.isEmpty())
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, TEXT_SAMENOTFOUND, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    ioTextWind.setResult(result);
    ioTextWind.setWindowTitle(WIND_NAME_RESULT);
    ioTextWind.exec();
}

void MainWindow::on_menuBar_byInformation_triggered()
{
    if(!QFileInfo::exists(currentGame->addonDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_ADDONDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    IOTextWindow ioTextWind;
    ioTextWind.setWindowTitle(WIND_NAME_FIND_INFO);
    ioTextWind.setWindowFlags(Qt::WindowCloseButtonHint);
    ioTextWind.setWindowIcon(QIcon(PATH_ICO_CHECKING));
    ioTextWind.exec();

    QString result = ioTextWind.getResult();
    if(result.isEmpty())
       return;

    setWaitingMode(true);

    result = checkInformation(result, currentGame->addonDir);

    setWaitingMode(false);

    if(result.isEmpty())
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, TEXT_NOSIMILARITY, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    ioTextWind.setResult(result);
    ioTextWind.setWindowTitle(WIND_NAME_RESULT);
    ioTextWind.exec();
}

void MainWindow::on_menuBar_depending_triggered()
{
    if(!QFileInfo::exists(currentGame->addonDir))
    {
        QMessageBox errorMessage(ICON_ERROR, TITLE_ERROR, TEXT_ADDONDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    if(!ui->listWidgetAddons->selectedItems().length())
    {
        QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_NOSELECTEADDON, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    setWaitingMode(true);

    QString result = checkDepending(getSelectedAddonPath(), currentGame->addonDir);

    setWaitingMode(false);

    if(result.isEmpty())
    {
        QMessageBox errorMessage(ICON_INFORMATION, TITLE_INFORMATION, TEXT_NODEPENDING, QMessageBox::Ok);
        errorMessage.exec();
        return;
    }

    IOTextWindow ioTextWind;
    ioTextWind.setResult(result);
    ioTextWind.setWindowTitle(WIND_NAME_RESULT);
    ioTextWind.setWindowFlags(Qt::WindowCloseButtonHint);
    ioTextWind.setWindowIcon(QIcon(PATH_ICO_CHECKING));
    ioTextWind.exec();
}

/*
// resize signal (curentWindowSize - nust be QSize in this class)
void MainWindow::resizeEvent(QResizeEvent *e)
{
    if(curentWindowSize.isEmpty())
        curentWindowSize = this->size();

    curentWindowSize.setHeight(curentWindowSize.height() + (this->width() - curentWindowSize.width()));
    curentWindowSize.setWidth(this->width());
    this->resize(curentWindowSize);

    this->resize(this->width(), this->width() * ((double)(curentWindowSize.height()) / curentWindowSize.width()));

    QWidget::resizeEvent(e);
}
*/

/*
// ccatch pessed key signal, without arrows key...
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, "Super check", QMessageBox::Ok);
    errorMessage.exec();
}
*/

void MainWindow::keyReleaseEvent(QKeyEvent *eventKey)
{
    int keyCode = eventKey->key();
    QKeySequence check(Qt::CTRL + Qt::Key_P);

    switch(keyCode)
    {
    case Qt::Key_Up:
    case Qt::Key_Down:
        if(ui->listWidgetAddons->selectedItems().count())
        {
            int index = ui->listWidgetAddons->currentRow();
            if(keyCode == Qt::Key_Up && index > 0)
                ui->listWidgetAddons->setCurrentRow(index - 1);
            else if(keyCode == Qt::Key_Down && index < ui->listWidgetAddons->count() - 1)
                ui->listWidgetAddons->setCurrentRow(index + 1);
            on_listWidgetAddons_clicked();
        }
        break;

    case Qt::Key_Plus:
        if(eventKey->modifiers() & Qt::ControlModifier)
            on_menuBar_plus25_triggered();
        break;

    case Qt::Key_Minus:
        if(eventKey->modifiers() & Qt::ControlModifier)
            on_menuBar_minus12_5_triggered();
        break;

    case Qt::Key_Backspace:
        if(ui->pushButtonLastAddon->isEnabled())
            on_pushButtonLastAddon_clicked();
        break;

    case Qt::Key_F:
        if(eventKey->modifiers() & Qt::ControlModifier)
            on_menuBar_byFileName_triggered();
        break;
    }
}

//////////////// My Functions ////////////////

void MainWindow::loadSettings()
{
    QFile settingFile(PATH_SETTING);
    QTextStream* readFile = 0;
    if (settingFile.open(QIODevice::ReadOnly))
       readFile = new QTextStream(&settingFile);

    for(auto iter = allGames; iter <= allGames + gamesCounter - 1; iter++)
    {
        if(!readFile || readFile->atEnd())
        {
            iter->addonDir = "";
            iter->gameDir = "";
            continue;
        }
        else
            iter->gameDir = readFile->readLine();

        if(!readFile || readFile->atEnd())
        {
            iter->gameDir = "";
            continue;
        }
        else
            iter->addonDir = readFile->readLine();
    }

    delete readFile;
    if(settingFile.isOpen())
        settingFile.close();
}

bool MainWindow::loadConfig()
{
    QFile configFile(PATH_CONFIG);
    QTextStream* readFile = 0;
    if (configFile.open(QIODevice::ReadOnly))
       readFile = new QTextStream(&configFile);

    if(!readFile || readFile->atEnd())
    {
        delete readFile;
        if(configFile.isOpen())
            configFile.close();
        return false;
    }

    QString buff;
    while(!readFile->atEnd())
    {
        buff = readFile->readLine();
        if(buff.isEmpty())
            continue;
        buff = buff.mid(buff.indexOf('=') + 1);
        gamesCounter = buff.mid(buff.indexOf('=') + 1).toInt();
        break;
    }

    if(!gamesCounter)
    {
        delete readFile;
        if(configFile.isOpen())
            configFile.close();
        return false;
    }


    allGames = new GameClass[gamesCounter];

    for(int iter = 0; iter < gamesCounter; iter++)
        allGames[iter].name = parseNextConfig(readFile);

    for(int iter = 0; iter < gamesCounter; iter++)
        allGames[iter].usedAddonsFile = parseNextConfig(readFile);

    for(int iter = 0; iter < gamesCounter; iter++)
        allGames[iter].logoFile = parseNextConfig(readFile);

    for(int iter = 0, numExe; iter < gamesCounter; iter++)
    {
        numExe = parseNextConfig(readFile).toInt();
        for(int jter = 0; jter < numExe; jter++)
            allGames[iter].pathExe.push_back(parseNextConfig(readFile));

    }

    delete readFile;
    if(configFile.isOpen())
        configFile.close();

    return true;
}

QString MainWindow::parseNextConfig(QTextStream *readFile)
{
    QString buff;
    while(readFile && !readFile->atEnd())
    {
        buff = readFile->readLine();
        if(buff.isEmpty())
            continue;

        return buff.mid(buff.indexOf('=') + 1).trimmed();
    }

    return "";
}

QString MainWindow::getSelectedAddonPath()
{
    QString addonPath = "/";

    if(ui->comboBoxType1->currentIndex() >= 0)
    {
        addonPath += ui->comboBoxType1->currentText() + "/";

        if(ui->comboBoxType2->currentIndex() >= 0)
        {
            addonPath += ui->comboBoxType2->currentText() + "/";

            if(ui->comboBoxType3->currentIndex() >= 0)
                addonPath += ui->comboBoxType3->currentText() + "/";
        }
    }

    return addonPath + ui->listWidgetAddons->currentItem()->text() + "/";
}

QString MainWindow::getSelectedAddonPathFull()
{
    return currentGame->addonDir + getSelectedAddonPath();
}

void MainWindow::setImage(QLabel* lable, QString path)
{
    QPixmap pix(path);

    pix = pix.scaledToWidth(lable->width());

    if(pix.height() > lable->height())
    {
        QPixmap buffPix(path);
        pix = buffPix.scaledToHeight(lable->height());
    }

    ui->labelImagePreview->setPixmap(pix);
}

void MainWindow::setTypes(QString dir)
{
    if(!dir.section('/', 2, 2).isEmpty())
    {
        ui->comboBoxType1->setCurrentText(dir.section('/', 1, 1));

        if(!dir.section('/', 3, 3).isEmpty())
        {
            ui->comboBoxType2->setCurrentText(dir.section('/', 2, 2));

            if(!dir.section('/', 4, 4).isEmpty())
                ui->comboBoxType3->setCurrentText(dir.section('/', 3, 3));
        }
    }
    else
        ui->comboBoxType1->setCurrentIndex(-1);


    QString buff = dir.section('/', -2, -2);
    for(int iter = 0; iter < ui->listWidgetAddons->count(); iter++)
    {
        if(ui->listWidgetAddons->item(iter)->text() == buff)
        {
            ui->listWidgetAddons->setCurrentRow(iter);
            break;
        }
    }

    if(ui->listWidgetAddons->selectedItems().length())
        on_listWidgetAddons_clicked();
}

void MainWindow::fillTypes(QComboBox *cb, QString path)
{
    cb->clear();

    QDir checkDir(path);
    if(checkDir.exists() && checkDir.isAbsolute())
    {
        checkDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        if(checkDir.entryList().length())
        {
            QDir buff;
            buff.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
            for(auto iter : checkDir.entryList())
            {
                buff.setPath(path + "/" + iter);
                if(buff.exists() && buff.isAbsolute())
                {
                    if(buff.entryList().length())
                        cb->addItem(iter);
                }
            }
        }
    }

    if(cb->count())
    {
        cb->setCurrentIndex(-1);
        cb->setVisible(true);
    }
    else
    {
        if(cb != ui->comboBoxType1)
            cb->setVisible(false);
    }
}

void MainWindow::fillAddons(QString path)
{
    QDir checkDir(path);
    if(checkDir.exists() && checkDir.isAbsolute())
    {
        checkDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        if(checkDir.entryList().length())
        {
            QDir buff;
            buff.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
            for(auto iter : checkDir.entryList())
            {
                buff.setPath(path + "/" + iter);
                if(buff.exists() && buff.isAbsolute())
                {
                    if(!buff.entryList().length())
                        ui->listWidgetAddons->addItem(iter);
                }
            }
        }
    }
}

void MainWindow::loadDescrip()
{
    QFile descripFile(getSelectedAddonPathFull() + NAME_FILE_DESCRIP);
    if (descripFile.open(QIODevice::ReadOnly))
    {
       QTextStream readFile(&descripFile);

       ui->textBrowserDescrip->setText(readFile.readAll());

       descripFile.close();
    }
}

void MainWindow::rebootAddonsType()
{
    fillTypes(ui->comboBoxType1, currentGame->addonDir);

    ui->listWidgetAddons->clear();
    ui->listWidgetDepend->clear();
    ui->textBrowserDescrip->clear();

    fillAddons(currentGame->addonDir);
}

void MainWindow::fillUsedAddons(QString pathList)
{
    ui->listWidgetUsing->clear();

    QFile addonsFile(pathList);
    if (addonsFile.open(QIODevice::ReadWrite))
    {
       QTextStream readFile (&addonsFile);
       while(!readFile.atEnd())
           ui->listWidgetUsing->addItem(readFile.readLine());
    }

    if(addonsFile.isOpen())
        addonsFile.close();
}

void MainWindow::reloadGameInfo()
{
    setImage(ui->labelImagePreview, PATH_APP + currentGame->logoFile);
    ui->pushButtonOpenView->setVisible(false);
    ui->pushButtonOpenDirectory->setVisible(false);

    lastAddon->current = "";
    lastAddon->last = "";
    ui->pushButtonLastAddon->setEnabled(false);

    rebootAddonsType();
    fillUsedAddons(PATH_APP + currentGame->usedAddonsFile);

    ui->pushButtonStartGame->setEnabled(!currentGame->gameDir.isEmpty());
}

QString MainWindow::getAddonDir(QString addonType)
{
    if(addonType.isEmpty())
        addonType = ui->comboBoxType1->currentText().toLower();
    else
        addonType = addonType.toLower();

    if(addonType == QString(KEY_CAMPAING).toLower())
        return KEY_CAMPAING_DIR;
    else if(addonType == QString(KEY_MISSION).toLower())
        return KEY_MISSION_DIR;

    return KEY_ADDON_DIR;
}

QString MainWindow::getAddonName(QString addonPath)
{
    int ind = addonPath.lastIndexOf('/');

    if(ind == addonPath.length()-1)
    {
        addonPath.remove(ind, 1);
        ind = addonPath.lastIndexOf('/');
    }

    return addonPath.right(addonPath.length() - ind - 1);
}

bool MainWindow::checkCopyAddon(QString addonPath)
{
    if(currentGame->gameDir.isEmpty())
    {
        QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_GAMEDIRWRONG, QMessageBox::Ok);
        errorMessage.exec();
        return false;
    }

    if(ui->listWidgetUsing->findItems(addonPath, nullptr).length())
    {
        QString addName = "\n" + getAddonName(addonPath);
        QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_ADDONADDED + addName, QMessageBox::Ok);
        errorMessage.exec();
        return false;
    }

    QDir dirAddon(currentGame->addonDir + addonPath);
    dirAddon.setNameFilters(QStringList(EXTENSION_ADDON));

    if(!dirAddon.entryList().length())
    {
        QString addName = "\n" + getAddonName(addonPath);
        QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_NOADDONFILES + addName, QMessageBox::Ok);
        errorMessage.exec();
        return false;
    }

    return true;
}

bool MainWindow::copyAddonFile(QString file, QString addonType, QString srcDir)
{
    if(srcDir.isEmpty())
        srcDir = getSelectedAddonPathFull();

    QDir targetDir(currentGame->gameDir + addonType);
    if(!targetDir.exists())
    {
        if(!targetDir.mkdir(targetDir.path()))
        {
            QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_CANTCREATEDIR + targetDir.path(), QMessageBox::Ok);
            errorMessage.exec();
            return false;
        }
    }

    if(!QFile::copy(srcDir + file, targetDir.path() + "/" + file))
    {
        if(QFileInfo::exists(targetDir.path() + "/" + file))
        {
            QString addName = getAddonName(srcDir);
            QMessageBox errorMessage(ICON_QUESTION, TITLE_ATTENTION, TEXT_FILEEXISTS + addName + "/" + file + TEXT_WANTREPLACE, QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
            int res = (errorMessage.exec());
            switch(res)
            {
            case QMessageBox::Yes:
                if(!QFile::remove(targetDir.path() + "/" + file))
                {
                    QString addName = getAddonName(srcDir);
                    QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_CANTDELETEFILE + addName + "/" + file, QMessageBox::Ok);
                    errorMessage.exec();
                    return false;
                }
                else
                    copyAddonFile(file, addonType, srcDir);
            case QMessageBox::No:
                break;
            default:
                return false;
            }
        }
        else
        {
            QString addName = getAddonName(srcDir);
            QMessageBox errorMessage(ICON_ATTENTION, TITLE_ATTENTION, TEXT_CANTCOPYFILE + addName + "/" + file, QMessageBox::Ok);
            errorMessage.exec();
            return false;
        }
    }

    return true;
}

void MainWindow::saveUsedList()
{
    QFile usedAddonsFile(PATH_APP + currentGame->usedAddonsFile);

    if (usedAddonsFile.open(QIODevice::WriteOnly))
    {
       QTextStream writeFile (&usedAddonsFile);

        for(int iter = 0; iter < ui->listWidgetUsing->count(); iter++)
            writeFile << ui->listWidgetUsing->item(iter)->text() << endl;

        usedAddonsFile.close();
    }
}

void MainWindow::setScale(QWidget *obj, double scale)
{
    obj->setGeometry(obj->x() * scale, obj->y() * scale,
                     obj->size().width() * scale, obj->size().height() * scale);

    QFont buffFont(obj->font());
    if(buffFont.pointSize() * scale < WINDOW_MIN_FONT)
        return;

    buffFont.setPointSize(buffFont.pointSize() * scale);
    obj->setFont(buffFont);
}

void MainWindow::changeScale(double scale)
{
    if(this->width() * scale < WINDOW_MIN_WIDTH)
        return;

    this->setFixedSize(this->width() * scale, this->height() * scale);

    setScale(ui->comboBoxType1, scale);
    setScale(ui->comboBoxType2, scale);
    setScale(ui->comboBoxType3, scale);

    setScale(ui->labelAddons, scale);
    setScale(ui->labelDepend, scale);
    setScale(ui->labelDescrip, scale);
    setScale(ui->labelType, scale);
    setScale(ui->labelUsed, scale);

    setScale(ui->labelImagePreview, scale);

    setScale(ui->listWidgetAddons, scale);
    setScale(ui->listWidgetDepend, scale);
    setScale(ui->listWidgetUsing, scale);

    setScale(ui->pushButtonAddAddon, scale);
    setScale(ui->pushButtonAddDepend, scale);
    setScale(ui->pushButtonDeleteUsed, scale);
    setScale(ui->pushButtonG1, scale);
    setScale(ui->pushButtonG2, scale);
    setScale(ui->pushButtonG3, scale);
    setScale(ui->pushButtonLastAddon, scale);
    setScale(ui->pushButtonOpenDirectory, scale);
    setScale(ui->pushButtonOpenView, scale);
    setScale(ui->pushButtonSetting, scale);
    setScale(ui->pushButtonStartGame, scale);

    setScale(ui->textBrowserDescrip, scale);

    if(ui->listWidgetAddons->selectedItems().length())
        on_listWidgetAddons_clicked();
    else
        setImage(ui->labelImagePreview, PATH_APP + currentGame->logoFile);
}

QString MainWindow::checkAddonsIn(QString srcPath, QString destPath)
{
    QString result = "";

    QDir addonsDir(srcPath);

    if(addonsDir.exists() && addonsDir.isAbsolute())
    {
        addonsDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        if(addonsDir.entryList().length())
        {
            for (auto iter : addonsDir.entryList())
                result += checkAddonsIn(srcPath + "/" + iter, destPath);
        }

        addonsDir.setFilter(QDir::Files);
        addonsDir.setNameFilters(QStringList(EXTENSION_ADDON));

        QString check;
        for(auto iter : addonsDir.entryList())
        {
            check = checkFileIn(iter, destPath);
            if(!check.isEmpty() && check != (addonsDir.path() + "/" + iter))
                result += check + "\n=\n" + addonsDir.path() + "/" + iter + "\n\n";
        }
    }

    return result;
}

QString MainWindow::checkAddonsIn(QString srcPath)
{
    QString result = "";

    QString destPath = srcPath;
    destPath.remove(0, 1);
    destPath = destPath.left(destPath.indexOf('/'));
    destPath = getAddonDir(destPath);

    QDir srcDir(currentGame->addonDir + srcPath);
    QDir destDir(currentGame->gameDir + destPath);

    if(srcDir.exists() && srcDir.isAbsolute() && destDir.exists() && destDir.isAbsolute())
    {
        bool check;
        srcDir.setNameFilters(QStringList(EXTENSION_ADDON));
        destDir.setNameFilters(QStringList(EXTENSION_ADDON));
        for(auto iter : srcDir.entryList())
        {
            check = true;
            for(auto jter : destDir.entryList())
            {
                if(iter.toLower() == jter.toLower())
                {
                    check = false;
                    break;
                }
            }

            if(check)
            {
                if(result.isEmpty())
                    result = "Addon \"" + srcPath + "\" have missed files:\n";
                result += "- " + iter + "\n";
            }
        }
    }

    return result;
}

QString MainWindow::checkFileIn(QString fileName, QString destPath)
{
    QString result = "";

    QDir destDir(destPath);

    if(destDir.exists() && destDir.isAbsolute())
    {
        destDir.setFilter(QDir::Files);
        destDir.setNameFilters(QStringList(EXTENSION_ADDON));
        for (auto iter : destDir.entryList())
        {
            if(iter.toLower() == fileName.toLower())
            {
                result += destDir.absolutePath() + "/" + iter;
                break;
            }
        }
        destDir.setNameFilters(QStringList(EXTENSION_ALL));
        destDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        if(result.isEmpty() && destDir.entryList().length())
        {
            for( auto iter : destDir.entryList() )
            {
                result += checkFileIn(fileName, destDir.path() + "/" + iter);
                if(!result.isEmpty())
                    break;
            }
        }
    }

    return result;
}

QString MainWindow::checkInformation(QString info, QString destPath)
{
    QString result = "";
    QString check;

    QDir destDir(destPath);
    QFile* descripFile;
    QTextStream* readFile;

    if(destDir.exists() && destDir.isAbsolute())
    {
        destDir.setFilter(QDir::Files);
        destDir.setNameFilters(QStringList(NAME_FILE_DESCRIP));
        for (auto iter : destDir.entryList())
        {
            descripFile = new QFile(destDir.absolutePath() + "/" + NAME_FILE_DESCRIP);

            if (descripFile->open(QIODevice::ReadOnly))
            {
               readFile = new QTextStream(descripFile);
               check = readFile->readAll();
               descripFile->close();
               delete readFile;

               if(check.toLower().indexOf(info.toLower()) >= 0)
                   result += "In Description: " + destDir.absolutePath() + "/\n";
            }

            delete descripFile;
        }
        destDir.setNameFilters(QStringList(EXTENSION_ALL));
        destDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        if(destDir.entryList().length())
        {
            for( auto iter : destDir.entryList() )
                result += checkInformation(info, destDir.path() + "/" + iter);
        }
        else
        {
            if(getAddonName(destPath).toLower().indexOf(info.toLower()) >= 0)
                result += "In Name: " + destDir.absolutePath() + "/\n";
        }
    }

    return result;
}

QString MainWindow::checkDepending(QString addon, QString destPath)
{
    QString result = "";
    QString check;

    QDir destDir(destPath);
    QFile* descripFile;
    QTextStream* readFile;

    if(destDir.exists() && destDir.isAbsolute())
    {
        destDir.setFilter(QDir::Files);
        destDir.setNameFilters(QStringList(NAME_FILE_DEPEND));
        for (auto iter : destDir.entryList())
        {
            descripFile = new QFile(destDir.absolutePath() + "/" + NAME_FILE_DEPEND);

            if (descripFile->open(QIODevice::ReadOnly))
            {
               readFile = new QTextStream(descripFile);
               check = readFile->readAll();
               descripFile->close();
               delete readFile;

               if(check.indexOf(addon) >= 0)
                   result += destDir.absolutePath() + "/\n";
            }

            delete descripFile;
        }
        destDir.setNameFilters(QStringList(EXTENSION_ALL));
        destDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        if(destDir.entryList().length())
        {
            for( auto iter : destDir.entryList() )
                result += checkDepending(addon, destDir.path() + "/" + iter);
        }
    }

    return result;
}

void MainWindow::setWaitingMode(bool isOn)
{
    if(isOn)
    {
        setImage(ui->labelImagePreview, PATH_WAITSCREEN);
    }
    else
    {
        if(ui->listWidgetAddons->selectedItems().length())
            on_listWidgetAddons_clicked();
        else
            setImage(ui->labelImagePreview, PATH_APP + currentGame->logoFile);
    }

    ui->labelImagePreview->repaint();
}

bool MainWindow::openDirectory(QString path)
{
    QDir gameDir(path);

    if(gameDir.exists() && gameDir.isAbsolute())
        return QDesktopServices::openUrl(QUrl::fromLocalFile(path));

    return false;
}

void MainWindow::on_menuBar_bugsFile_triggered()
{
    QString buff = PATH_BUGFILE;

    if(!QFileInfo::exists(buff))
    {
        QFile fileCreator(buff);
        fileCreator.open(QIODevice::WriteOnly | QIODevice::Text);
        fileCreator.close();
    }

    QProcess::startDetached(APP_NOTEPAD + buff);
}

void MainWindow::on_menuBar_hotKeys_triggered()
{
    QString text = "Hot Keys:\n"
                   "\n"
                   "(Up), (Down)\t- Select Addons from \"Addon List\"\n"
                   "(Ctrl)+(+)\t- Scale + 25%\n"
                   "(Ctrl)+(-)\t- Scale - 12.5%\n"
                   "(BackSpace)\t- Last selected Addon\n"
                   "(Ctrl)+(F)\t- Check addon files by name\n";

    QMessageBox infoMessage(ICON_INFORMATION, TITLE_INFORMATION, text, QMessageBox::Ok);
    infoMessage.exec();
}

void MainWindow::on_menuBar_about_triggered()
{
    QString text = "About:\n"
                   "\n"
                   "Program Name: Operation Addon\n"
                   "Version: 2.1\n"
                   "Date: 22.08.2017\n"
                   "\n"
                   "Kyiv, Ukraine\n"
                   "email: stalker2021@ukr.net\n"
                   "\n"
                   "This program created for organization of work with addons.\n"
                   "Suported games: \"Operation Flashpoint: Cold War Crisis\", \"Operation Flashpoint: Resistance\", "
                   "\"ArmA: Cold War Assault\", \"ArmA: Armed Assault\", \"ArmA: Queen's Gambit\", \"ArmA 2\", "
                   "\"ArmA 2: Operation Arrowhead\".\n"
                   "\n"
                   "Not recommended change file structure of addons!\n"
                   "For more information, contact me by email.";

    QMessageBox infoMessage(ICON_INFORMATION, TITLE_INFORMATION, text, QMessageBox::Ok);
    infoMessage.exec();
}
