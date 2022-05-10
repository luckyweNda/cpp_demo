#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main()
{
    int listenfd = -1;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd == -1)
    {
        printf("Create socket error.\n");
        return 0;
    }

    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(51000);

    int check = -1;
    check = bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr));

    if(check == -1)
    {
        printf("Bind socket error.\n");
        return 0;
    }

    check = listen(listenfd, 10);

    if(check == -1)
    {
        printf("Listen error.\n");
        return 0;
    }

    printf("======Waiting for client message======\n");

    while(true)
    {
        int connfd = -1;
        connfd = accept(listenfd, (sockaddr*)NULL, NULL);

        if(connfd == -1)
        {
            printf("Accept connection failed.\n");
            continue;
        }

        int n = 0;
        char buffer[256];

        n = recv(connfd, buffer, sizeof(buffer), 0);
        buffer[n] = '\0';   //  IMPORTANT!!

        printf("Receive a message from client:\n%s\n", buffer);
        close(connfd);
    }

    close(listenfd);
    return 0;
}