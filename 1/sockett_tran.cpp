#include<stdio.h>
#include<string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>
#include "my_insert.h"
#include"select.h"
using namespace std;
using namespace rapidjson;
#define MYPORT 7007
#define MAXLINE 4096
char buf[200];
struct sockaddr_in sin_addr,pin_addr;
#define BUFFER_SIZE 4096
#define MAX_THREADS 5
FILE *fp;
void *User_connect(void *args)
{
    int new_sockfd = *(int *)args;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    printf("4\n");
    int n;
    while (1)
    {
        //退出程序
        printf("5\n");
        if (strcmp(buffer, "exit") == 0)
        {
            pthread_exit(NULL);
            break;
        }
        n=read(new_sockfd,buffer,MAXLINE);
        if(n==0)
        {
            printf("6\n");
            break;
        }
        fwrite(buffer,1,n,fp);
        printf("7\n");
    }
    buffer[n]='\0';
    printf("recv msg from client:%s\n",buffer);
    close(new_sockfd);
    printf("8\n");
    fclose(fp);
    printf("9\n");
}

int main(int argc, char const *argv[])
{
    //套接字类型、TCP还是UDP（SOCK_STREAM是TCP，表示链接的；SOCK_DGRAM是UDP，表示无连接），协议（一般设置为0，表示默认）
    int sockfd,new_sockfd;
    int len,pin_addr_size=0,i;
    if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf ("can't create socket\n");
        exit(1);
    }  
    memset(&sin_addr,0,sizeof(sin_addr));
    sin_addr.sin_family=AF_INET;
    sin_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    sin_addr.sin_port=htons(MYPORT);
    //bind：把电脑真正的网络地址和套接字绑定在一起
    if (bind(sockfd,(struct sockaddr*)&sin_addr,sizeof(sin_addr))<0)
    {
        printf("can't bind socket\n");
        exit(1);
    }
    else
    {
        printf("[Server is ready.]\n");
    }

    //listen：bind后，queue_size（允许多少个链接请求排入队列）
    if (listen(sockfd,100)<0)                        //监听
    {
        printf("listen error\n");
        exit(1);
    }
    else
    {
        printf("server is listening.\n");
    }
    
    if((fp=fopen("ssh.txt","ab"))==NULL)
    {
        printf("open file failed.\n");
    }
    printf("1\n");
    while (1)
    {
        //限制连接量,未完成
        printf("2\n");
        //客户端套接字

        //accept接受服务器的套接字 多客户端需要多线程
        if ((new_sockfd=accept(sockfd,(struct sockaddr*)&pin_addr,(socklen_t *)&pin_addr_size))<0)
            {
                printf("accept error\n");
                exit(1);   
            } 
        printf("3\n");
        //连接成功提示
        pthread_t tid;
        pthread_create(&tid, NULL, User_connect, (void *)&(new_sockfd));
    }
    return 0;
}
