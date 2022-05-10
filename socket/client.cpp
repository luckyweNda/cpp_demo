#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: ./client 127.0.0.1\n");
        return 0;
    }

    int sockfd = -1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        printf("Create socket failed.\n");
        return 0;
    }

    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);

    int check = -1;
    check = inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if(check <= 0)
    {
        printf("inet_pton error.\n");
        return 0;
    }

    check = connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

    if(check == -1)
    {
        printf("Connect failed.\n");
        return 0;
    }

    printf("It's time to send message to server.\n");

    char buffer[256];
    fgets(buffer, sizeof(buffer), stdin);

    check = send(sockfd, buffer, strlen(buffer), 0);

    if(check == -1)
    {
        printf("Send message error.\n");
        return 0;
    }

    close(sockfd);
    return 0;
}