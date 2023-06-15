#include "controller.h"
#include <stdio.h>


Controller::Controller()
{
    mySer = new Server();
}

Controller::~Controller()
{
    delete mySer;
}

QByteArray Controller::opMsg(QByteArray input, string& name,
                             string& roomName, string& YorN, int& kind){
    QString str1 = input;
    string str2 = str1.toStdString();
    str2 = str2.substr(0, str2.size()-1);
    cout << "[str2 in controller.cpp]" << str2 << endl; ////////////////////
    str2 = mySer->communication(str2, name, roomName, kind);
    cout << "[out123456]" << str2;
    YorN = str2;
    str2 = str2 + '\n';
    str1 = QString::fromStdString(str2);
    QByteArray str3 = str1.toLatin1();
    return str3;
}
