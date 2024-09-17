#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QList>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include "controller.h"
#include "mtcpsocket.h"

using std::string;
using std::cout;
using std::endl;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private:
    //控制器
    Controller* myCon;                   //控制器

    QLabel *lab_listen;

    QTcpServer *tcp_server;              //服务端

    QTcpSocket *tcp_socket;              //当前连接
    MTcpSocket *mtcp_socket;

    QList<MTcpSocket *>  mtcp_sockets;   //所有的连接都保存在这里

    QList<QTcpSocket *>  tcp_sockets;    //所有的连接都保存在这里

    map<string, set<QTcpSocket*> > rN2Soc; //roomNameToSocket
protected:
    //当服务端关闭
    void closeEvent(QCloseEvent *event);
private slots:
    //客户端断开时
    void onClientDisconnected(MTcpSocket *);
    //客户端连接时
    void onClientConnected();
    //客户端发来数据时
    void onSocketReadyRead();//读取数据
    //有新的客户端接入
    void onNewConnection();//有的新的连接

    //服务端开始监听
    void on_listen_clicked();
    //停止监听
    void on_shutdown_clicked();
    //发送信息
    void on_send_clicked();
};

#endif // MAINWINDOW_H
