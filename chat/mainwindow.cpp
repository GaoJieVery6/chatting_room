/*1. 删除registerin界面，注册和登录的用户名密码输入都在mainwindow内输入，出错弹出警告框。*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_clientdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->label->setStyleSheet("background:transparent;border:2px solid red;");
      /*设置背景*/
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(main_pic));
    setPalette(pal);
    this->setStyleSheet("QMainWindow{border-radius:15px;}");

    //this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
    ui->setupUi(this);

    /*设置标签字体，第一个参数是字体（微软雅黑），第二个是字体大小(单位为pt)*/
    QFont font("Comic Sans MS",15);
    ui->lreg->setFont(font);
    ui->llog->setFont(font);
    ui->lcode->setFont(font);
    ui->luse->setFont(font);
    ui->lins->setFont(font);
    QFont nfont("Comic Sans MS",13);
    ui->exit->setFont(nfont);

    /*按钮设成圆角*/
    ui->pushButton->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->dengLu->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->pushButton_3->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->userName ->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->userPassword ->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->exit ->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");

     /*按钮设成透明*/
    ui->pushButton->setFlat(true);
    ui->dengLu->setFlat(true);
    ui->pushButton_3->setFlat(true);

    /*设置按钮图案*/
    QString sheet1 = "border-image: url(" + button1_pic + ");";
    QString sheet2 = "border-image: url(" + button2_pic + ");";
    QString sheet3 = "border-image: url(" + button4_pic + ");";
    ui->pushButton->setStyleSheet(sheet1);
    ui->pushButton_3->setStyleSheet(sheet2);
    ui->dengLu->setStyleSheet(sheet3);

    ui->userPassword->setEchoMode(QLineEdit::Password);                     //隐藏密码
    // RoundRect();

    tcpClient = new QTcpSocket; //创建socket变量
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    QString     addr = "127.0.0.1";
    quint16     port = 7777;
    tcpClient->connectToHost(addr, port);
}

MainWindow::~MainWindow()
{
    delete tcpClient;
    delete new_client;
    delete new_login;
    delete new_ins;
    delete ui;
    exit(0);
}

bool check(QString test){
    for(auto i = test.begin(); i != test.end(); ++ i){
        if((*i>='0'&&*i<='9')||(*i>='a'&&*i<='z')||(*i>='A'&&*i<='Z'))
            ;
        else
            return false;
    }
    return true;
}

void MainWindow::RoundRect(){                  //将窗口设为圆角
    QBitmap bmp(this->size());
    bmp.fill(this,0,0);
    QPainter p(&bmp);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(bmp.rect(),20,20,Qt::AbsoluteSize);
    setMask(bmp);
}

/*点击login按钮，注册新用户，跳转到聊天室登录界面*/
void MainWindow::on_dengLu_clicked()
{
    QString msg = ui->userName->text();
    QString msg2 = ui->userPassword->text();
    _input_name = msg.toStdString();
    QString test = msg + msg2;

    if(!check(test))
        Warning::getWarning(this, VALUE);
    else{
        msg = "SD" + msg + " " + msg2;
        QByteArray  str = msg.toUtf8();
        str.append('\n');
        string temp = str.toStdString();

        cout << "[out]" << temp << endl;
        tcpClient->write(str);            //向服务器发送用户登录请求
    }
}

/*点击register按钮，用户登录，跳转到聊天室登录界面*/
void MainWindow::on_pushButton_clicked()
{
    QString msg = ui->userName->text();
    QString msg2 = ui->userPassword->text();
    _input_name = msg.toStdString();
    string password=msg2.toStdString();
    QString test = msg + msg2;

    if(!check(test))
        Warning::getWarning(this, VALUE);       //判断密码是否为空
    else{
        msg = "SZ" + msg + " " + msg2;
        QByteArray  str = msg.toUtf8();
        str.append('\n');
        string temp = str.toStdString();

        cout << "[out]" << temp << endl;
        tcpClient->write(str);                           //向服务器发送注册请求
    }
}

/*点击instruction 按钮，跳转到使用说明界面*/
void MainWindow::on_pushButton_3_clicked()
{
    this->hide();
    new_ins=new instruction;
    connect(new_ins,SIGNAL(sendsignal()),this,SLOT(show()));
    new_ins->show();
}

void MainWindow::onSocketReadyRead() {                 //聊天 收消息
    //readyRead()信号槽函数
    if(tcpClient->canReadLine()) {
        QByteArray input = tcpClient->readLine();
        QString str1 = input;
        string str2 = str1.toStdString();

        cout << "[in]" << str2 << endl;
        if(str2 == "YL\n"){//服务器返回确认注册消息，跳转到聊天室登录界面
            _name = _input_name;
            this->hide();
            new_login=new login;
            connect(new_login,SIGNAL(sendsignal()),this,SLOT(show()));
            new_login->show();
        }
        else if(str2 == "YD\n"){  //服务器返回确认登录消息，跳转到聊天室登录界面
            _roomName = _input_roomName;
            this->hide();
            new_login->hide();
            new_client=new clientDialog;
            connect(new_client,SIGNAL(sendsignal()),new_login,SLOT(show()));
            new_client->show();
            new_client->flush_timer->start();
        }
        else if(str2[0] == 'M'){
            str2 = str2.substr(1);
            string msgroom;
            stringstream s(str2);
            s >> msgroom;
            str2 = str2.substr(msgroom.size());
            str1 = QString::fromStdString(str2);
            input = str1.toLatin1();
            if(msgroom == _roomName)
                new_client->ui->output->appendPlainText(input);
        }
         /*服务器返回错误信息，显示错误提示窗口*/
        else if(str2 == "NP\n")
            Warning::getWarning(this, PASSWORD);
        else if(str2 == "NO\n")
            Warning::getWarning(this, REPEAT);
        else if(str2 == "NN\n")
            Warning::getWarning(this, EXISTED);
        else if(str2[0] == 'U'){                     /*显示服务器返回的当前聊天室成员信息*/
            cout << "[UserInfo]" << str2 << endl;
            new_client->ui->chengyuan->setText(str1);
            clientinfo=str1;
        }
        else if(str2[0] == 'R'){               /*服务器返回的聊天室信息，打开聊天室信息界面*/
            //str2显示到room.ui上
            cout << str2 << endl;
            roominfo=str1;
            new_room=new room;
            new_room->show();
        }
    }
}

/*点击exit按钮，关闭当前客户端*/
void MainWindow::on_exit_clicked()
{
    this->close();
    exit(0);
}
