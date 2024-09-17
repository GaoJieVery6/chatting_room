#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QBitmap>
#include <QPainter>
#include <string>
#include <iostream>

#include "clientdialog.h"
#include "ui_mainwindow.h"
#include "room.h"
#include "const.h"
#include "warning.h"

using std::string;
using std::cout;
using std::endl;

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    clientDialog* new_client;
    room* new_room;

private slots:
    void on_create_clicked();

    void on_exit_clicked();

    void on_enter_clicked();

    void RoundRect();

    void on_pushButton_clicked();

private:
    Ui::login *ui;

signals:
    void sendsignal();
};

#endif // LOGIN_H
