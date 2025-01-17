#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QShortcut>
#include <QKeyEvent>
#include <QBitmap>
#include <QPainter>
#include <string>
#include <iostream>
#include "const.h"

using std::string;
using std::cout;
using std::endl;

namespace Ui {
class clientDialog;
}

class clientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit clientDialog(QWidget *parent = nullptr);
    ~clientDialog();

private slots:
    void on_pushButton_clicked();

    void on_send_clicked();

    //void on_input_textChanged();

    void RoundRect();
    void on_shuaxin_clicked();

public:
    Ui::clientDialog *ui;


protected:
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器

signals:
    void sendsignal();
};

#endif // CLIENTDIALOG_H
