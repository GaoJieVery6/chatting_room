#include "warning.h"

const QString Warning::font_default = "Comic Sans MS";
const QString Warning::fontsize_default = "5";

Warning::Warning()
{

}

void Warning::getWarning(QWidget* widget, ERROR e)
{
    if(e == VALUE)
        QMessageBox::warning(widget, "Warning", makeHTML("Input can not be null!"), QMessageBox::Ok);
    else if(e == PASSWORD)
        QMessageBox::warning(widget, "Warning", makeHTML("Password wrong!"), QMessageBox::Ok);
    else if(e == REPEAT)
        QMessageBox::warning(widget, "Warning", makeHTML("User has logined!"), QMessageBox::Ok);
    else if(e == EXISTED)
        QMessageBox::warning(widget, "Warning", makeHTML("Name has existed!"), QMessageBox::Ok);
}

QString Warning::makeHTML(QString content, QString font, QString fontsize)
{
    if(font.isEmpty())
        font = font_default;
    if(fontsize.isEmpty())
        fontsize = fontsize_default;
    QString temp = "<font size=\'" + fontsize + "\'" + " face=\'" + font + "\'>" + content + "</font>";
    QString html = QObject::tr(temp.toStdString().c_str());
    //qDebug() << html ;
    return html;
}
