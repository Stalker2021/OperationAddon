#ifndef IOTEXTWINDOW_H
#define IOTEXTWINDOW_H

#include <QDialog>
#include <QString>

#include "defines.h"

namespace Ui {
class IOTextWindow;
}

class IOTextWindow : public QDialog
{
    Q_OBJECT

public:
    explicit IOTextWindow(QWidget *parent = 0);
    ~IOTextWindow();

    QString getResult();
    void setResult(QString resultStr);

private slots:
    void on_IOTextWindow_windowTitleChanged(const QString &title);

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::IOTextWindow *ui;

    QString result;

    void selectMode(bool isResult);
};

#endif // IOTEXTWINDOW_H
