#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QFont>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <string>
#include <sstream>
#include <iostream>

#include "clientdialog.h"
#include "instruction.h"
#include "room.h"
#include "login.h"
#include "const.h"
#include "warning.h"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void RoundRect();

private:
    Ui::MainWindow *ui;
    clientDialog* new_client;
    login* new_login;
    instruction* new_ins;
    room* new_room;

private slots:
    //服务端发来了信息
    void onSocketReadyRead();

    void on_dengLu_clicked();

    void on_exit_clicked();
};

#endif // MAINWINDOW_H
