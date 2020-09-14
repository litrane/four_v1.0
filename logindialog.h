#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include"head.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class logindialog; }
QT_END_NAMESPACE

class logindialog : public QDialog
{
    Q_OBJECT
public:
    logindialog(QWidget *parent = nullptr);
    ~logindialog();
    int sock_cli;
     struct sockaddr_in servaddr;

private slots:
     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

private:
    Ui::logindialog *ui;
signals:
    void sendData(QString a1,QString a2);   //用来传递数据的信号

};
#endif // LOGINDIALOG_H
