/*部分说明：
 * 1.当一个socket断开连接后，要删除tcp_sockets可变长数组和rN2Soc中的相应的socket,
 * 这具体地体现在void MainWindow::onClientDisconnected()函数中 。
 */

// 头函数引用部分
#include "mainwindow.h"
#include "ui_mainwindow.h"

// 函数体部分
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    lab_listen = new QLabel("监听状态:");
    lab_listen->setMinimumWidth(150);
    ui->statusBar->addWidget(lab_listen);

    ui->listen->setEnabled(true);
    ui->shutdown->setEnabled(false);

    tcp_server = new QTcpServer(this);

    //当server有新的连接时，响应到 onNewConnection()函数
    connect(tcp_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    myCon = new Controller();

    this->setWindowTitle("server");
}

MainWindow::~MainWindow() {
    delete myCon;
    delete ui;
}

void MainWindow::on_listen_clicked() {
    QString IP = ui->server_ip->text(); //IP地址
    quint16 port = ui->server_port->text().toInt(); //端口
    QHostAddress addr(IP);

    //开始监听
    tcp_server->listen(addr, port);


    ui->information->appendPlainText("**开始监听...");
    ui->information->appendPlainText("**服务器地址："
                                     + tcp_server->serverAddress().toString());
    ui->information->appendPlainText("**服务器端口："
                                     + QString::number(tcp_server->serverPort()));

    ui->listen->setEnabled(false);
    ui->shutdown->setEnabled(true);

    lab_listen->setText("监听状态：正在监听");
}

void MainWindow::onNewConnection() {
    tcp_socket = tcp_server->nextPendingConnection(); //创建socket
    mtcp_socket = new MTcpSocket();
    mtcp_socket->setTcpSocket(tcp_socket);
    tcp_sockets.push_back(tcp_socket);//将新的连接放进list 便于管理
    mtcp_sockets.push_back(mtcp_socket);

    connect(tcp_socket, SIGNAL(connected()), this, SLOT(onClientConnected()));

    onClientConnected();

    connect(mtcp_socket, SIGNAL(disconnected(MTcpSocket *)),
            this, SLOT(onClientDisconnected(MTcpSocket *)));

    connect(tcp_socket, SIGNAL(readyRead()),
            this, SLOT(onSocketReadyRead()));
}

void MainWindow::onClientConnected() {
    //客户端接入时
    ui->information->appendPlainText("**client socket connected");
    ui->information->appendPlainText("**peer address:" +
                                     tcp_socket->peerAddress().toString());
    ui->information->appendPlainText("**peer port:" +
                                     QString::number(tcp_socket->peerPort()));
}

void MainWindow::onClientDisconnected(MTcpSocket *socket) {

    ui->information->appendPlainText("**client socket disconnected");
    qDebug() << "delete";

    tcp_sockets.removeOne(socket->getTcpScoket());
    mtcp_sockets.removeOne(socket);
    ui->information->appendPlainText("**client socket disconnected");

    for(auto i = rN2Soc.begin(); i != rN2Soc.end(); ++ i){
        string roomName = i->first;
        set<QTcpSocket*> Soc = i->second;
        int ccount = Soc.size();
        for(auto j = Soc.begin(); j != Soc.end(); ++j){
            int flag = 0;
            for(auto k = tcp_sockets.begin(); k != tcp_sockets.end(); ++k){
                if(*j == *k)
                    flag = 1;
            }
            if(!flag)
                ccount -- ;
        }
        if(ccount == 0)
            (myCon->mySer->rNP).erase(roomName);

    }

    map<QTcpSocket*, string> reg = myCon->mySer->reg;
    for(auto i = reg.begin(); i != reg.end(); ++ i){
        int flag = 0;
        for(auto j = tcp_sockets.begin(); j != tcp_sockets.end(); ++ j){
            if(i->first == *j)
                flag = 1;
        }
        if(flag == 0 && myCon->mySer->online.count(i->second)){
            myCon->mySer->online.erase(i->second);
        }
    }
}

void MainWindow::onSocketReadyRead() {
    //服务端收到了信息
    for(auto i = tcp_sockets.begin(); i != tcp_sockets.end(); ++i) {//循环判断是哪一个客户端发来的
        tcp_socket = *i;
        while(tcp_socket->canReadLine()){//是否有可读的信息
            QByteArray str = tcp_socket->readLine();
            string name, roomName, YorN;
            int kind;
            QByteArray str1 = myCon->opMsg(str, name, roomName, YorN, kind);

            ui->information->appendPlainText("[in] " + str);
            if(kind == 4){
                tcp_socket->write(str1);
            }
            if(kind == 1){
                tcp_socket->write(str1);
                if(YorN[0] == 'Y'){
                    myCon->mySer->reg[tcp_socket] = name;
                }
            }
            if(kind == 3){
                tcp_socket->write(str1);
                if(YorN[0] == 'Y'){
                    rN2Soc[roomName].insert(tcp_socket);
                }
            }
            if(kind == 2){ //转发到同聊天室所有客户端
                set<QTcpSocket*> tcp = rN2Soc[roomName];
                for(auto j = tcp.begin(); j != tcp.end(); ++j) {
                    (*j)->write(str1);
                }
            }
        }
    }
}

void MainWindow::on_shutdown_clicked() {
    //停止监听
    if (tcp_server->isListening()) { //tcpServer正在监听
        tcp_server->close();//停止监听
        ui->listen->setEnabled(true);
        ui->shutdown->setEnabled(false);
        lab_listen->setText("监听状态：已停止监听");
    }
}

void MainWindow::on_send_clicked() {
    //发送一行字符串，以换行符结束
    QString  msg = ui->message->toPlainText();
    ui->information->appendPlainText("[out] " + msg);
    ui->message->clear();
    ui->message->setFocus();

    QByteArray  str = msg.toUtf8();
    str.append('\n');//添加一个换行符

    //将信息发送给每一个 客户端
    for(auto i = tcp_sockets.begin(); i != tcp_sockets.end(); ++i) {
        (*i)->write(str);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    //关闭窗口时停止监听
    if (tcp_server->isListening())
        tcp_server->close();;//停止网络监听
    event->accept();
}
