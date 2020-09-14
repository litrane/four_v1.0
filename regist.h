#ifndef REGIST_H
#define REGIST_H
#include"head.h"

namespace Ui {
class regist;
}

class regist : public QDialog
{
    Q_OBJECT

public:
    explicit regist(QWidget *parent = nullptr);
    ~regist();
int sock_cli;
 struct sockaddr_in servaddr;
private slots:
    void on_pushButton_clicked();

private:
    Ui::regist *ui;
};

#endif // REGIST_H
