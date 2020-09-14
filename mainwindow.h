#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"head.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
  //  void newFile();   // 新建操作
  //  bool maybeSave(); // 判断是否需要保存
    bool save();      // 保存操作
    bool saveAs();    // 另存为操作
   bool saveFile(const QString &fileName); // 保存文件
 //   bool isUntitled;
    // 保存当前文件的路径
    QString curFile="file/1.txt";
    int sock_cli;
    struct sockaddr_in servaddr;

private slots:
    void on_pushButton_clicked();
 void receiveData(QString data,QString data2);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
