#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MYPORT 4003
#define MAXLINE 1000
char buf[200];
FILE *fp;
int main (int argc,char* argv[])
{
    int sockfd,new_sockfd;
    struct sockaddr_in sin_addr,pin_addr;
    int len,pin_addr_size,i;
    if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf ("can't create socket\n");
        exit(1);
    }
    memset(&sin_addr,0,sizeof(sin_addr));
    sin_addr.sin_family=AF_INET;
    sin_addr.sin_addr.s_addr=inet_addr("192.168.43.88");
    sin_addr.sin_port=htons(MYPORT);
    if (connect(sockfd,(struct sockaddr*)&sin_addr,sizeof(sin_addr))<0)
    {
        printf("can't connect socket\n");
        exit(1);
    }
        scanf ("%s",buf);
        if ((len=send(sockfd,buf,200,0))<0)
        {
            printf("send error\n");
            exit(1);
        }
        int n;
        char buffer[200];
    if((fp=fopen("./base/key.txt","ab"))==NULL)
    {
        printf("open file failed.\n");
    }
    while(1)
    {
/*    if (recv(sockfd,buf,200,0)==-1)
        {
            printf ("can't receive packet\n");
            exit(1);
        }
        printf ("From server: %s\n",buf);*/
        printf("5\n");
        n=read(sockfd,buffer,MAXLINE);
        if(n==0)
        {
            printf("6\n");
            break;
        }
        fwrite(buffer,1,n,fp);
        printf("7\n");
    }
    buffer[n]='\0';
    close(sockfd);  
    fclose(fp);
    return 0;                 
}