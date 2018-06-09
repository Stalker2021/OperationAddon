#include "iotextwindow.h"
#include "ui_iotextwindow.h"

IOTextWindow::IOTextWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IOTextWindow)
{
    ui->setupUi(this);

    result = "";
}

IOTextWindow::~IOTextWindow()
{
    delete ui;
}

void IOTextWindow::on_IOTextWindow_windowTitleChanged(const QString &title)
{
    selectMode(title == WIND_NAME_RESULT);

    if(title == WIND_NAME_FIND_FILE)
        ui->lineEdit_find->setText(EXTENSIONONLY_ADDON);
}

void IOTextWindow::on_pushButton_cancel_clicked()
{
    result = "";
    this->close();
}

void IOTextWindow::on_pushButton_ok_clicked()
{
    result = ui->lineEdit_find->text();
    this->close();
}

//////////////// My Functions ////////////////

QString IOTextWindow::getResult()
{
    return result;
}

void IOTextWindow::setResult(QString resultStr)
{
    result = resultStr;
}

void IOTextWindow::selectMode(bool isResult)
{
    ui->textEdit_result->setVisible(isResult);

    ui->pushButton_cancel->setVisible(!isResult);
    ui->label_find->setVisible(!isResult);
    ui->lineEdit_find->setVisible(!isResult);

    if(isResult)
        ui->textEdit_result->setText(result);
    else
        this->resize(this->width(), this->minimumHeight());
}
