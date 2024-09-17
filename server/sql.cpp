#include "sql.h"

const QString SQL::HOST = "localhost";
const int SQL::PORT = 3306;
const QString SQL::DB = "chat";
const QString SQL::TABLE_USER = "user";
const QString SQL::TABLE_RECORD = "record";
//const QString SQL::TABLE_ROOM = "room";
const QString SQL::USERNAME = "root";
const QString SQL::PWD = "";
const QString SQL::CREATE_TABLE_USER =
        "create table if not exists " + TABLE_USER +
        "(\
          ID varchar(20) not null primary key,\
          Code varchar(20) not null\
         )default charset = utf8";

/*const QString SQL::CREATE_TABLE_ROOM =
        "create table if not exists " + TABLE_ROOM +
        "(\
          ID varchar(20) not null primary key,\
          Code varchar(20) not null\
          foreign key(Room) references room(ID) on delete cascade on update cascade\
         )default charset = utf8";*/

const QString SQL::CREATE_TABLE_RECORD =
        "create table if not exists " + TABLE_RECORD +
        "(\
          ID varchar(20) not null,\
          Room varchar(20) not null,\
          Msg varchar(50),\
          Date datetime,\
          foreign key(ID) references user(ID) on delete cascade on update cascade\
         )default charset = utf8";


SQL::SQL()
{

}

SQL::~SQL()
{
    database.close();
}

bool SQL::initialize()
{
    if(!MODE.empty()){
        connectDB();
        createDB();
        return true;
    }
    else
        qDebug() << "Error: The database does not initialize correctly.";
    return false;
}

void SQL::setMode(string md)
{
    if(md == "Server")
        MODE = md;
    else if(md == "Client")
        MODE = md;
    else
        qDebug() << "Error: The mode is incorrect.";
}

bool SQL::connectDB()
{
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(HOST);
    database.setPort(PORT);
    database.setUserName(USERNAME);
    database.setPassword(PWD);
    if(!database.open()){
        qDebug() << database.lastError().text();
        database.close();
        return false;
    }
    return true;
}

bool SQL::createDB()
{
    QString querystring;
    querystring = "create database if not exists " + DB;
    database.exec(querystring);
    if(database.lastError().isValid()){
        qDebug() << database.lastError().text();
        return false;
    }

    database.setDatabaseName(DB);
    if(!database.open()){
        qDebug() << database.lastError().text();
        return false;
    }

    if(MODE == "Server"){
        database.exec(CREATE_TABLE_USER);
        if(database.lastError().isValid()){
            qDebug() << database.lastError().text();
            return false;
        }
    }

    if(MODE == "Server"){
        database.exec(CREATE_TABLE_RECORD);
        if(database.lastError().isValid()){
            qDebug() << database.lastError().text();
            return false;
        }
    }

    return true;
}

bool SQL::createUser(QString user, QString code)
{
    if(MODE == "Server"){
        QSqlQuery query;
        query.prepare("insert into " + TABLE_USER + " (ID,Code) VALUES (?,?)");
        query.addBindValue(user);
        query.addBindValue(code);
        if(query.exec())
            return true;
        else{
            qDebug() << query.lastError().text();
            return false;
        }
    }
    return false;
}

bool SQL::deleteUser(QString user)
{
    if(MODE == "Server"){
        QSqlQuery query;
        query.prepare("delete from " + TABLE_USER + " where ID = ?");
        query.addBindValue(user);
        if(query.exec())
            return true;
        else{
            qDebug() << query.lastError().text();
            return false;
        }
    }
    return false;
}

bool SQL::updateCode(QString user, QString code)
{
    if(MODE == "Server"){
        QSqlQuery query;
        query.prepare("update " + TABLE_USER + " Code = ? where ID = ?");
        query.addBindValue(code);
        query.addBindValue(user);
        if(query.exec())
            return true;
        else{
            qDebug() << query.lastError().text();
            return false;
        }
    }
    return false;
}

bool SQL::exists(QString user)
{
    if(MODE == "Server"){
        QSqlQuery query;
        query.prepare("select ID from " + TABLE_USER + " where ID = ?");
        query.addBindValue(user);
        if(query.exec()){
            if(!query.size())
                return false;
            return true;
        }
        else{
            qDebug() << query.lastError().text();
            return false;
        }
    }
    return false;
}

bool SQL::login(QString user, QString code)
{
    if(MODE == "Server"){
        QSqlQuery query;
        query.prepare("select Code from " + TABLE_USER + " where ID = ?");
        query.addBindValue(user);
        if(query.exec()){
            if(!query.size())
                return false;
            query.next();
            if(query.value(0).toString() == code)
                return true;
            else
                return false;
        }
        else{
            qDebug() << query.lastError().text();
            return false;
        }
    }
    return false;
}

bool SQL::insertMSG(QString user, QString room, QString msg)
{
    if(MODE == "Server"){
        QSqlQuery query;
        query.prepare("insert into " + TABLE_RECORD + " (ID,Room,Msg,Date) VALUES (?,?,?,?)");
        query.addBindValue(user);
        query.addBindValue(room);
        query.addBindValue(msg);
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_time = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
        query.addBindValue(current_time);
        if(query.exec())
            return true;
        else{
            qDebug() << query.lastError().text();
            return false;
        }
    }
    return false;
}
