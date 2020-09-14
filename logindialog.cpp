#include "logindialog.h"
#include "ui_logindialog.h"
#include "regist.h"
#define BUFFER_SIZE 1024
QString username;
using namespace std;
logindialog::logindialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::logindialog)
{
    ///定义sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("192.168.43.88");  ///服务器ip

    ///连接服务器，成功返回0，错误返回-1
    if (::connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connectdvss");
        exit(1);
    }

    ui->setupUi(this);
}

logindialog::~logindialog()
{
    delete ui;
}


void logindialog::on_pushButton_clicked()
{
    QJsonObject json;
   json.insert("usrname",ui->usrLineEdit->text());
   json.insert("pwd",ui->pwdLineEdit->text());
   json.insert("status",1);
   json.insert("command",2);
   QJsonDocument document;
   document.setObject(json);
   QByteArray byteArray = document.toJson(QJsonDocument::Compact);
   QString strJson(byteArray);
   string sendbuf= strJson.toStdString();
    const char* p = sendbuf.data();
    qDebug()<<p;

   send(sock_cli, p, strlen(p),0);
   char recvbuf[BUFFER_SIZE];
   if(recv(sock_cli, recvbuf, sizeof(recvbuf),0)==-1)
   {
       qDebug()<<"error";
   }
    qDebug()<<recvbuf;

   QJsonParseError jsonParserError;
      QJsonDocument   jsonDocument =QJsonDocument::fromJson( recvbuf, &jsonParserError );
      if(jsonParserError.error != QJsonParseError::NoError)
         {
             qDebug() << jsonParserError.error;
             return;
         }
      qDebug() << QString("文件解析成功\n");
          if ( jsonDocument.isObject() ) {
              QJsonObject jsonObject = jsonDocument.object();
              if ( jsonObject.contains( "status" )  ) {
                   if(jsonObject["status"]==1)
                   {
                       QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("通知"),QString("登录成功"));
                       QTimer::singleShot(1000,m_box,SLOT(accept()));
                       m_box->exec();
                       this->accept();
 username=ui->usrLineEdit->text();

                   }
                   else
                   {
                       qDebug()<< "登陆失败\n";
                   }
              }



}

         ::close(sock_cli);
}

void logindialog::on_pushButton_2_clicked()
{
    regist *dlg = new regist(this);
       dlg->show();
}
