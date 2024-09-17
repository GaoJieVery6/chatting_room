#include "server.h"

Server::Server()
{
    sql = new SQL();
    sql->setMode("Server");
    sql->initialize();
}

Server::~Server()
{
    delete sql;
}

string Server::communication(string input, string& name, string& roomName, int& kind)
{
    string password, roomPassword;
    if(input[0] == 'F'){
        string input2 = input.substr(1);
        stringstream s(input2);
        string nname, nrome;

        s >> nname >> nrome;
        cout << "chu fang:"<<nname << " " << nrome << endl;
        cout << "RS:" << Room["1"].size() << endl;
        for(auto ii = Room.begin(); ii != Room.end(); ++ii){
            cout << "hhh" << endl;
            cout << "[" << ii->first << "]" << endl;
        }
        set<string> m = Room[nrome];
        cout << "MS:" << m.size() << endl;
        Room[nrome].erase(nname);
        return "";
    }
    else if(input[0] == 'D'){
        string name = input.substr(1);
        online.erase(name);
        return "";
    }
    else if(input[0] == 'S'){
        if(input[1] == 'D'){ // 登(Deng)录
            kind = 1;        // kind=1，返回mainwindow.cpp后不需要任何处理
            string temp = input.substr(2);
            stringstream s(temp);
            s >> name >> password;

            QString qtemp1 = QString::fromStdString(name);
            QString qtemp2 = QString::fromStdString(password);

            if(!online.count(name)){
                if(sql->exists(qtemp1)&&sql->login(qtemp1,qtemp2)){
                    online.insert(name);
                    return "YL";
                }
                else
                    return "NP";
            }
            else
                return "NO"; // 重复登录 No Online
        }
        if(input[1] == 'Z'){ // 注(Zhu)册
            kind = 1;
            string temp = input.substr(2);
            stringstream s(temp);
            s >> name >> password;

            QString qtemp1 = QString::fromStdString(name);
            QString qtemp2 = QString::fromStdString(password);

            if(!sql->exists(qtemp1)){
                online.insert(name);
                sql->createUser(qtemp1, qtemp2);
                return "YL";
            }
            else
                return "NN";
        }
        if(input[1] == 'C'){ //  请求创建(Create)某聊天室
            // kind=3 && return="Y"，返回mainwindow.cpp后需要把name用户的*socket加入到roomName的集合中
            kind = 3;
            string temp = input.substr(2);
            stringstream s(temp);
            s >> name >> roomName >> roomPassword;

            if(!rNP.count(roomName)){  // 此房间原来不存在，可以创建
                rNP[roomName] = roomPassword;
                Room[roomName].insert(name);
                cout << "RSize:" << Room[roomName].size() << endl;
                return "YD";
            }
            else
                return "NN";
        }
        if(input[1] == 'E'){ // 请求进入(Enter)某聊天室
            kind = 3;
            //cout << "[input in communication]" << input << endl;
            string temp = input.substr(2);
            stringstream s(temp);
            s >> name >> roomName >> roomPassword;
            //cout << "enter##" << name << "##" << roomName << "##" << roomPassword << endl; /////////////////
            if(rNP.count(roomName) && rNP[roomName] == roomPassword){
                Room[roomName].insert(name);
                return "YD";
            }
            else
                return "NP";
        }
    }
    else if(input[0] == 'M'){
        // kind=2，向roomName集合内的所有*socket发送形式为[name]:input的消息
        kind = 2;
        string temp = input.substr(1);
        stringstream s(temp);
        s >> name >> roomName;
        int len = name.size() + roomName.size() + 1;
        input = temp.substr(len);
        string ret = "M"+roomName+" "+"[" + name + "] : " + input;
        QString qtemp1 = QString::fromStdString(name);
        QString qtemp2 = QString::fromStdString(roomName);
        QString qtemp3 = QString::fromStdString(input);
        sql->insertMSG(qtemp1, qtemp2, qtemp3);
        return ret;
    }
    else if(input[0] == 'R'){
        string ret = "Room:";
        for(auto i = rNP.begin(); i != rNP.end(); ++i){
            if(i->second=="1")
                ret = ret + "[" + i->first + ":public]  ";
            else
                ret = ret + "[" + i->first + ":private]  ";
        }
        kind = 4;
        return ret;

    }
    else if(input[0] == 'U'){
        kind = 4;
        string ret = "Users:";
        string rName = input.substr(1);
        for(auto i = Room.begin(); i != Room.end(); ++ i){
            if(i->first == rName){
                set<string> member = i->second;
                for(auto j = member.begin(); j != member.end(); ++ j){
                    if(online.count(*j)){
                        //cout << "*JJJJJJ" << endl; ///////////////////////////////////////
                        ret = ret + "["+*j + "]  ";
                    }
                }
            }
        }
        return ret;
    }
}
