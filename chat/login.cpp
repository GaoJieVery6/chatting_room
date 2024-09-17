#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    /*加背景*/
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(main_pic));
    setPalette(pal);
    //this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
    ui->setupUi(this);

    /*设置标签字体*/
    QFont font("Comic Sans MS",15);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
    ui->label_5->setFont(font);
    QFont nfont("Comic Sans MS",13);
    ui->exit->setFont(nfont);

    /*按钮设成圆角*/
    ui->exit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->pushButton->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->create->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->enter->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");

      /*按钮设成透明*/
     ui->pushButton->setFlat(true);
    ui->create->setFlat(true);
    ui->enter->setFlat(true);

    /*设置按钮图案*/
    QString sheet1 = "border-image: url(" + button1_pic + ");";
    QString sheet2 = "border-image: url(" + button2_pic + ");";
    QString sheet3 = "border-image: url(" + button4_pic + ");";
    ui->pushButton->setStyleSheet(sheet1);
    ui->create->setStyleSheet(sheet2);
    ui->enter->setStyleSheet(sheet3);

     /*设置显示框为圆角*/
    ui->roomPassword->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->roomName->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->roomPassword->setEchoMode(QLineEdit::Password);         //密码显示
    //RoundRect();
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(MainWindow::onSocketReadyRead()));
}

login::~login()
{
    delete ui;
}

bool check1(QString test){
    for(auto i = test.begin(); i != test.end(); ++ i){
        if((*i>='0'&&*i<='9')||(*i>='a'&&*i<='z')||(*i>='A'&&*i<='Z'))
            ;
        else
            return false;
    }
    return true;
}

void login::RoundRect(){                  //将窗口设为圆角
    QBitmap bmp(this->size());
    bmp.fill(this,0,0);

    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(bmp.rect(),20,20,Qt::AbsoluteSize);
    setMask(bmp);
}

/*点击exit按钮时，退出聊天室登录界面，返回主界面*/
void login::on_exit_clicked()
{
    QString msg = QString::fromStdString(_name);
    msg = "D" + msg;
    QByteArray str = msg.toUtf8();
    str.append('\n');
    string temp = str.toStdString();

    cout << "[out]" << temp << endl;
    tcpClient->write(str);
    emit sendsignal();
    this->close();

}

/*点击enter按钮，进入聊天室，跳转到聊天界面*/
void login::on_enter_clicked()
{
    QString msg = ui->roomName->text();
    QString msg2 = ui->roomPassword->text();
    _input_roomName = msg.toStdString();
    QString test = msg + msg2;

    if(!check1(test))
        Warning::getWarning(this, VALUE);
    else{
        msg = "SE" + QString::fromStdString(_name) + " " + msg + " " + msg2;
        QByteArray  str = msg.toUtf8();
        str.append('\n');
        string temp = str.toStdString();

        cout << "[out]" << temp << endl;
        tcpClient->write(str);
    }
}

/*点击create按钮，建立新聊天室*/
void login::on_create_clicked()
{
    QString msg = ui->roomName->text();
    QString msg2 = ui->roomPassword->text();
    _input_roomName = msg.toStdString();
    string password=msg2.toStdString();
    QString test = msg + msg2;

    if(!check1(test))
        Warning::getWarning(this, VALUE);   //判断密码是否为空
    else{
        msg = "SC" + QString::fromStdString(_name) + " " + msg + " " + msg2;
        QByteArray  str = msg.toUtf8();
        str.append('\n');
        string temp = str.toStdString();

        cout << "[out]" << temp << endl;
        tcpClient->write(str);             //若用户名不为空，将用户名和密码发送给服务器
    }
}

/*点击room按钮，显示已建聊天室的信息*/
void login::on_pushButton_clicked()
{
    QString msg;
    msg = "R";
    QByteArray  str = msg.toUtf8();
    str.append('\n');

    cout << "[out]" << str.toStdString() << endl;
    tcpClient->write(str);                  //向服务器发送请求，得到聊天室信息

}
