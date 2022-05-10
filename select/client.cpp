#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int m_sockfd = -1;
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(m_sockfd == -1)
    {
        printf("Create socket error\n");
        return 0;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(51000);

    int connectfd = -1;
    connectfd = connect(m_sockfd, (sockaddr *)&server_addr, sizeof(server_addr));

    if(connectfd == -1)
    {
        printf("Connect server failed.\n");
        return 0;
    }

    while(true)
    {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));

        printf("Enter the sentence you want to send to server.\n");
        fgets(buffer, sizeof(buffer), stdin);
        send(m_sockfd, buffer, sizeof(buffer), 0);
    }

    close(m_sockfd);
    return 0;
}