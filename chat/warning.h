#ifndef WARNING_H
#define WARNING_H
#include <QMessageBox>
#include <QWidget>
#include <QString>
#include <QObject>
#include <QDebug>

enum ERROR
{
    VALUE,      //值为空
    PASSWORD,   //密码不正确
    REPEAT,     //重复登陆
    EXISTED     //账号存在
};

class Warning
{
public:
    Warning();
    static void getWarning(QWidget* widget, ERROR e);
private:
    static const QString font_default;
    static const QString fontsize_default;
    static QString makeHTML(QString content, QString font = "", QString fontsize = "");
};

#endif // WARNING_H
