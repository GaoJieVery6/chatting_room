#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QDialog>
#include <QBitmap>
#include <QPainter>

#include "const.h"

namespace Ui {
class instruction;
}

class instruction : public QDialog
{
    Q_OBJECT

public:
    explicit instruction(QWidget *parent = nullptr);
    ~instruction();

private:
    Ui::instruction *ui;
    void RoundRect();

signals:
    void sendsignal();
private slots:
    void on_pushButton_clicked();
};

#endif // INSTRUCTION_H
