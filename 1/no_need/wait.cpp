#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main ()
{
    pid_t pid;
    pid_t childpid;
    if ((pid=fork())==0)
    {
        sleep(1);
        printf ("hello I am child\n");
    }
    else
    {
        childpid=wait(NULL);
        printf("helllo I am parent, child has finished:PID=%d\n",childpid);
    }
    exit(0);
    
}