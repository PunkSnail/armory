#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 1995

void MyError(int number, const char*p)
{
    if(number == -1)
    {
        printf("%s", p);
        perror(" error");

        exit(1);
    }
}

void Service(int cfd)
{
    char buf[1024];

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        int ret = read(cfd, buf, sizeof(buf));

        MyError(ret, "read");

        if(ret == 0)
        {
            printf("Client closed\n");

            break;
        }

        write(1, buf, ret);
        //回写客户端
        write(cfd, buf, ret);
    }
}

int main(void)
{
    //注册要捕捉的信号
    signal(SIGCHLD, SIG_IGN);

    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    MyError(lfd, "socket");

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //端口复用
    int s = 1;
    MyError(setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &s, sizeof(s)), "setsockopt");

    MyError(bind(lfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)), "bind");

    MyError(listen(lfd, 233), "listen");

    struct sockaddr_in clientAddr;

    int clientLen = sizeof(clientAddr);

    int cfd;

    pid_t pid;


    while(1)
    {
        cfd = accept(lfd, (struct sockaddr*)&clientAddr, &clientLen);

        MyError(cfd, "accept");


        pid = fork();

        MyError(pid, "fork");

        if(pid == 0)
        {
            printf("Client %s PORT %d\n",
                   inet_ntoa(clientAddr.sin_addr),
                   ntohs(clientAddr.sin_port));

            MyError(close(lfd), "close");

            Service(cfd);

            exit(1);
            //printf("The client disconnect....\n");
            //break;"
        }
        else if(pid > 0)
        {
            MyError(close(cfd), "close");
        }
    }

    return 0;
}
