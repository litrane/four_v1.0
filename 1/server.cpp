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
#include<mysql/mysql.h>
#include"email.h"
using namespace std;
using namespace rapidjson;
#define MYPORT 4002
#define BUFFER_SIZE 4096
#define MAXLINE 2048 
#define MAX_THREADS 100
struct sockaddr_in sin_addr,pin_addr;
FILE *fp;
int myselect(const char *password,const char* usrname)
{
    char client[] = "root";
    char server[] = "localhost";
    char  code[] ="root";
    MYSQL *sql = mysql_init(NULL);
    MYSQL_RES *sql_res;
    MYSQL_FIELD *field;
    if(sql == NULL)
    {
        printf("ERROR IN INIT! %s\n",mysql_error(sql));
        return -1;
    }
    if(NULL   == mysql_real_connect(sql,server,client,code,NULL,0,NULL,0) )
    {
        printf("ERROR IN CONNECT! %s\n",mysql_error(sql));
        mysql_close(sql);
        return -1;
    }
    /*
    if(mysql_query(sql,"CREATE DATABASE IF NOT EXSISTS mysqltest")){
        printf("ERROR IN CREATION!\n %s\n",mysql_error(sql));
        mysql_close(sql);
        return  -1;
    }
    */
    if( mysql_query(sql,"USE mysqltest") ) {
        printf("ERROR IN USE\n");
        mysql_close(sql);
        return -1;
    }
    char quest[100] = "SELECT password FROM login WHERE usrname = ";
    char tem[50];
    sprintf(tem,"'%s'",usrname);
    strcat(quest,tem);
    printf("%s\n",quest);
    if(mysql_query(sql,quest)){
        printf("ERROR IN SELECT %s",mysql_error(sql));
        mysql_close(sql);
        return -1;
    }
    sql_res = mysql_store_result(sql);

    if(mysql_num_rows(sql_res)==0){
        //printf("NO RESULT!\n");
        mysql_close(sql);
        return 0;
    }
    else {
        MYSQL_ROW row = mysql_fetch_row(sql_res);
        if(strcmp(row[0],password)==0) return 1;
	    else return 0;
    }    
    //printf ("%s\n",mysql_get_server_info(sql)  );
    //printf("%s\n",mysql_get_host_info(sql));
    mysql_close(sql);
    return 1;
}
int insert(const char *psw,const char *usrname){    
    char client[] = "root";
    char server[] = "localhost";
    char  code[] ="root";
    MYSQL *sql = mysql_init(NULL);
    MYSQL_RES *sql_res;
    MYSQL_FIELD *field;
    if(sql == NULL){
        printf("ERROR IN INIT! %s\n",mysql_error(sql));
        return -1;
    }
    if(NULL   == mysql_real_connect(sql,server,client,code,NULL,0,NULL,0)  ){
        printf("ERROR IN CONNECT! %s\n",mysql_error(sql));
        mysql_close(sql);
        return -1;
    }
    if( mysql_query(sql,"USE mysqltest") ) {
        printf("ERROR IN USE\n");
        mysql_close(sql);
        return -1;
    }
    char quest[] = "INSERT INTO login (usrname,  password) VALUES ";
    char values[200];
    sprintf(values,"('%s','%s')",usrname,psw);
    strcat(quest,values);
    if(mysql_query(sql,quest)){
        printf("ERROR IN INSERT! %s\n",mysql_error(sql));
        mysql_close(sql);
        return -1;  
    }
    printf ("%s\n",mysql_get_server_info(sql)  );
    printf("%s\n",mysql_get_host_info(sql));
    mysql_close(sql);
    return 0;
}
void *User_connect(void *args)
{
    int new_sockfd = *(int *)args;
    char buffer_[BUFFER_SIZE];
    memset(buffer_, 0, sizeof(buffer_));
    while (recv(new_sockfd, buffer_, BUFFER_SIZE, 0) > 0)
    {
        if (strlen(buffer_) == 0)
        {
            strcpy(buffer_, "end of connect");
            pthread_exit(NULL);
            continue;
        }
        printf ("%s\n",buffer_);
        string s=buffer_;
        string json = s;
        printf("%s",json.c_str());
        Document dom;                          //json解析
        if (!dom.Parse(json.c_str()).HasParseError()) 
        {
		    if (dom.HasMember("command") && dom["command"].IsInt()) 
            {   
                printf("1");
                switch (dom["command"].GetInt())
                {
                    printf("2");
                    case 1:                   //注册
                    {
                        if (insert(dom["pwd"].GetString(),dom["usrname"].GetString())==0)
                        {
                            StringBuffer buffer;
                            Writer<StringBuffer> Writer(buffer);
                            Writer.StartObject();
                            Writer.Key("status");
                            Writer.Int(1);         
                            Writer.EndObject();                  
                            string tosend=buffer.GetString();
                            char p[200];
                            int i;
                            for(i=0;i<=tosend.length()-1;i++)
                            {
                                p[i]=tosend[i];
                            }
                            p[i]='\0';
                            send(new_sockfd,p,200,0);
                        }
                        else
                        {
                            StringBuffer buffer;
                            Writer<StringBuffer> Writer(buffer);
                            Writer.StartObject();
                            Writer.Key("status");
                            Writer.Int(-1);         
                            Writer.EndObject();                  
                            string tosend=buffer.GetString();
                            char p[200];
                            int i;
                            for(i=0;i<=tosend.length()-1;i++)
                            {
                                p[i]=tosend[i];
                            }
                            p[i]='\0';
                            send(new_sockfd,p,200,0);
                        }
                        break;
                    }
                    case 2: //登录
                    {
                        int flag = myselect(dom["pwd"].GetString(),dom["usrname"].GetString());
                        if(flag==1)
                        {
                            StringBuffer buffer;
                            Writer<StringBuffer> Writer(buffer);
                            Writer.StartObject();
                            Writer.Key("status");
                            Writer.Int(1);         
                            Writer.EndObject();                  
                            string tosend=buffer.GetString();
                            char p[200];
                            int i;
                            for(i=0;i<=tosend.length()-1;i++)
                            {
                                p[i]=tosend[i];
                            }
                            p[i]='\0';
                            send(new_sockfd,p,200,0);
                        }
                        else if(flag == 0)
                        {
                            StringBuffer buffer;
                            Writer<StringBuffer> Writer(buffer);
                            Writer.StartObject();
                            Writer.Key("status");
                            Writer.Int(0);         
                            Writer.EndObject();                  
                            string tosend=buffer.GetString();
                            char p[200];
                            int i;
                            for(i=0;i<=tosend.length()-1;i++)
                            {
                                p[i]=tosend[i];
                            }
                            p[i]='\0';
                            send(new_sockfd,p,200,0);
                        }
                        else
                        {
                            StringBuffer buffer;
                            Writer<StringBuffer> Writer(buffer);
                            Writer.StartObject();
                            Writer.Key("status");
                            Writer.Int(-1);         
                            Writer.EndObject();                  
                            string tosend=buffer.GetString();
                            char p[200];
                            int i;
                            for(i=0;i<=tosend.length()-1;i++)
                            {
                                p[i]=tosend[i];
                            }
                            p[i]='\0';
                            send(new_sockfd,p,200,0);
                        }
                        break;
                    }   
                    case 3:             //receive mails
                    {
                        if (usr_check(dom["usrname"].GetString())) //success
                        {
                            string usrname_=dom["usrname"].GetString();
                            string receiver_=dom["receiver"].GetString();
                            string time_=dom["time"].GetString();
                            string filename_="./base/"+usrname_+"_"+receiver_+"_"+time_+".zip";
                            char filename[220];
                            int i;
                            for (i=0;i<=filename_.length()-1;i++)
                            {
                                filename[i]=filename_[i];
                            }
                            printf("1\n");
                            filename[i]='\0';
                            if((fp=fopen(filename,"ab"))==NULL)
                            {
                                printf("open file failed.\n");
                            }
                            printf("2\n");
                            StringBuffer buffer;
                            Writer<StringBuffer> Writer1(buffer);
                            Writer1.StartObject();
                            Writer1.Key("status");
                            Writer1.Int(1);         
                            Writer1.EndObject();                  
                            string tosend=buffer.GetString();
                            char p[200];
                            for(i=0;i<=tosend.length()-1;i++)
                            {
                                p[i]=tosend[i];
                            }
                            p[i]='\0';
                            send(new_sockfd,p,200,0);
                            int n;
                            char buffer_1[BUFFER_SIZE];
                            memset(buffer_1, 0, sizeof(buffer));
                            while (1)
                            {
                                //退出程序
                                if (strcmp(buffer_1, "exit") == 0)
                                {
                                    pthread_exit(NULL);
                                    break;
                                }
                                n=read(new_sockfd,buffer_1,MAXLINE);
                                if(n==0)
                                {
                                    break;
                                }
                                fwrite(buffer_1,1,n,fp);
                            }
                            buffer_1[n]='\0';
                            fclose(fp);
                            if(email_insert(dom["time"].GetString(),dom["usrname"].GetString(),dom["receiver"].GetString(),"aaa",'s','s')) //email restore success
                            {
                                printf("insert success!");
                                StringBuffer buffer_t;
                                Writer<StringBuffer> Writer2(buffer_t);
                                Writer2.StartObject();
                                Writer2.Key("status");
                                Writer2.Int(1);         
                                Writer2.EndObject();
                                tosend=buffer_t.GetString();
                                for(i=0;i<=tosend.length()-1;i++)
                                {
                                    p[i]=tosend[i];
                                }
                                p[i]='\0';
                                send(new_sockfd,p,200,0); 
                            }
                            else    //email restore failed
                            {
                                printf("insert failed!");
                                StringBuffer buffer_t;
                                Writer<StringBuffer> Writer2(buffer_t);
                                Writer2.StartObject();
                                Writer2.Key("status");
                                Writer2.Int(0);         
                                Writer2.EndObject();
                                tosend=buffer_t.GetString();
                                for(i=0;i<=tosend.length()-1;i++)
                                {
                                    p[i]=tosend[i];
                                }
                                p[i]='\0';
                                send(new_sockfd,p,200,0); 
                            }
                        }
                        else  //json analyze failed
                        {
                            StringBuffer buffer;
                            Writer<StringBuffer> Writer(buffer);
                            Writer.StartObject();
                            Writer.Key("status");
                            Writer.Int(0);         
                            Writer.EndObject();                  
                            string tosend=buffer.GetString();
                            char p[200];
                            int i;
                            for(i=0;i<=tosend.length()-1;i++)
                            {
                                p[i]=tosend[i];
                            }
                            p[i]='\0';
                            send(new_sockfd,p,200,0);
                        }
                        break;
                    }   
                    case 4:
                    {
                        string temp=Sql_Res_To_Json(email_select_receive(dom["receiver"].GetString()));
                        char p[2048];
                        int i;
                        for (i=0;i<=temp.length()-1;i++)
                        {
                            p[i]=temp[i];
                        }
                        p[i]='\0';
                        send(new_sockfd,p,2048,0);
                    }    
                }
		    }
        }
        else
        {
            StringBuffer buffer;
            Writer<StringBuffer> Writer(buffer);
            Writer.Key("status");
            Writer.Int(-1);                           
            const char *tosend=buffer.GetString();
            send(new_sockfd,tosend,200,0);
        }
        memset(buffer_, 0, sizeof(buffer_));
    }
}
int main(int argc, char const *argv[])
{
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
    if (bind(sockfd,(struct sockaddr*)&sin_addr,sizeof(sin_addr))<0)
    {
        printf("can't bind socket\n");
        exit(1);
    }
    else
    {
        printf("[Server is ready.]\n");
    }
    if (listen(sockfd,100)<0)                        //监听
    {
        printf("listen error\n");
        exit(1);
    }
    while (1)
    {
        if ((new_sockfd=accept(sockfd,(struct sockaddr*)&pin_addr,(socklen_t *)&pin_addr_size))<0)
        {
            printf("accept error\n");
            exit(1);   
        } 
        pthread_t tid;
        pthread_create(&tid, NULL, User_connect, (void *)&(new_sockfd));
    }
    return 0;
}
