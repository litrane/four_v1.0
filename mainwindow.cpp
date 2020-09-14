#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
#include <time.h>
#include <cstdlib>

#define BUFFER_SIZE 1024
extern QString username;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*void MainWindow::newFile()
{
   if (maybeSave()) {
       isUntitled = true;
       curFile = tr("未命名.txt");
       setWindowTitle(curFile);
       ui->textEdit->clear();
       ui->textEdit->setVisible(true);
   }
}
bool MainWindow::maybeSave()
{
    // 如果文档被更改了
    if (ui->textEdit->document()->isModified()) {
    // 自定义一个警告对话框
       QMessageBox box;
       box.setWindowTitle(tr("警告"));
       box.setIcon(QMessageBox::Warning);
       box.setText(curFile + tr(" 尚未保存，是否保存？"));
       QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                        QMessageBox::YesRole);
       box.addButton(tr("否(&N)"), QMessageBox::NoRole);
       QPushButton *cancelBut = box.addButton(tr("取消"),
                        QMessageBox::RejectRole);
       box.exec();
       if (box.clickedButton() == yesBtn)
            return save();
       else if (box.clickedButton() == cancelBut)
            return false;
   }
   // 如果文档没有被更改，则直接返回true
   return true;
}*/
bool MainWindow::save()
{
    QFile file("/home/dd/hello/four/file/1.txt");
       file.open(QIODevice::ReadWrite | QIODevice::Text);
QString e = ui->textEdit->toPlainText();
       file.write(e.toUtf8());
           file.close();
           return 1;
}
/*
bool MainWindow::saveAs()
{
   QString fileName = QFileDialog::getSaveFileName(this,
                                         tr("另存为"),curFile);
   if (fileName.isEmpty()) return false;
   return saveFile(fileName);
}
bool MainWindow::saveFile(const QString &fileName)
{
   QFile file(fileName);

   if (!file.open(QFile::WriteOnly | QFile::Text)) {

       // %1和%2分别对应后面arg两个参数，/n起换行的作用
       QMessageBox::warning(this, tr("多文档编辑器"),
                   tr("无法写入文件 %1：/n %2")
                  .arg(fileName).arg(file.errorString()));
       return false;
   }
   QTextStream out(&file);
   // 鼠标指针变为等待状态
   QApplication::setOverrideCursor(Qt::WaitCursor);
   out << ui->textEdit->toPlainText();
   // 鼠标指针恢复原来的状态
   QApplication::restoreOverrideCursor();
   //isUntitled = false;
   // 获得文件的标准路径
   curFile = QFileInfo(fileName).canonicalFilePath();
   setWindowTitle(curFile);
   return true;
}

*/
void MainWindow::on_pushButton_clicked()
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
    QJsonObject json;
   json.insert("usrname",username);
//   json.insert("pwd",ui->pwdLineEdit->text());
  // json.insert("status",1);
   json.insert("command",3);
   long long time_last;
       time_last = time(NULL);
       QString time_stick= QString::number(time_last,10);
   json.insert("time",time_stick);
   json.insert("receiver",ui->lineEdit->text());
   json.insert("theme",ui->lineEdit_2->text());
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
                        QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("通知"),QString("发送成功"));
                        QTimer::singleShot(1000,m_box,SLOT(accept()));
                        m_box->exec();
                       // emit ExitWin();
                        save();
                        string s="cd ~/hello/four/file && zip -q -r ../zip/";
                        s=s+username.toStdString();
                        s=s+'_';
                        s=s+ui->lineEdit->text().toStdString();
                        s=s+'_';
                       s=s+ QString::number(time_last,10).toStdString();
                        s=s+".zip . -i *";
                        qDebug()<<s.c_str();
 FILE *fq;
 char buffer[2048];
 int len;
 string s1="/home/dd/hello/four/zip/";
 s1=s1+username.toStdString();
 s1=s1+'_';
 s1=s1+ui->lineEdit->text().toStdString();
 s1=s1+'_';
s1=s1+ QString::number(time_last,10).toStdString();
s1=s1+".zip";
qDebug()<<s1.c_str();
                        system(s.c_str());
                        if((fq=fopen(s1.c_str(),"rb"))==NULL)
                          {
                              printf("File open failed.\n");
                              ::close(sock_cli);
                              exit(1);
                          }
                          bzero(buffer,sizeof(buffer));
                          while(!feof(fq))
                          {
                              len=fread(buffer,1,sizeof(buffer),fq);
                              if(len!=write(sock_cli,buffer,len))
                              {
                                  printf("write.\n");
                                  break;
                              }
                          }

                    }
                    else
                    {
                        qDebug()<< "登陆失败\n";
                    }
               }

::close(sock_cli);

 }



}
