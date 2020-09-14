#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MYPORT 6000
char buffer[2048];
int main (int argc,char* argv[])
{
    int sockfd,new_sockfd;
    struct sockaddr_in sin_addr,pin_addr;
    int len,pin_addr_size,i;
    FILE *fq;
    if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf ("can't create socket\n");
        exit(1);
    }
    memset(&sin_addr,0,sizeof(sin_addr));
    sin_addr.sin_family=AF_INET;
    sin_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    sin_addr.sin_port=htons(MYPORT);
    if (connect(sockfd,(struct sockaddr*)&sin_addr,sizeof(sin_addr))<0)
    {
        printf("can't connect socket\n");
        exit(1);
    }
    if((fq=fopen("key.txt","rb"))==NULL)
    {
        printf("File open failed.\n");
        close(sockfd);
        exit(1);
    }
    bzero(buffer,sizeof(buffer));
    while(!feof(fq))
    {
        len=fread(buffer,1,sizeof(buffer),fq);
        if(len!=write(sockfd,buffer,len))
        {
            printf("write.\n");
            break;
        }
    }
    close(sockfd);
    fclose(fq); 
    return 0;                 
}