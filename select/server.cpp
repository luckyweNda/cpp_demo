#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>

int main()
{
    int m_sockfd = -1;
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(m_sockfd == -1)
    {
        printf("Create socket error.\n");
        return 0;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //server_addr.sin_addr.s_addr = inet_addr("0.0.0.0"); //用这个写法也可以
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(51000);

    int m_bindfd = -1;
    m_bindfd = bind(m_sockfd, (sockaddr*)&server_addr, sizeof(server_addr));

    if(m_bindfd == -1)
    {
        printf("Bind socket error.\n");
        return 0;
    }

    int m_listenfd = -1;
    m_listenfd = listen(m_sockfd, 10);

    if(m_listenfd == -1)
    {
        printf("Listen socket error.\n");
        return 0;
    }

    /***      Starting to build select      ***/
    
    fd_set tmpfd;
    int max_fd = m_sockfd;
    std::vector<int> array_fd;
    sockaddr_in client_addr;
    int size = sizeof(client_addr);

    while(true)
    {
        FD_ZERO(&tmpfd);
        FD_SET(m_sockfd, &tmpfd);

        // 所有在线的客户端加入到fd中
        for(auto it = array_fd.begin(); it < array_fd.end(); it++)
        {
            FD_SET(*it, &tmpfd);
        }

        int ret = -1;
        ret = select(max_fd + 1, &tmpfd, NULL, NULL, NULL);

        if(ret == -1)
        {
            printf("select error.\n");
            continue;
        }
        else if(ret == 0)
        {
            printf("Select time out.\n");
            continue;
        }

        //表示有客户端连接
        if(FD_ISSET(m_sockfd, &tmpfd))
        {
            int m_connfd = -1;
            m_connfd = accept(m_sockfd, (struct sockaddr *)&client_addr,(socklen_t*)&size);

            if(m_connfd == -1)
            {
                printf("Accept connection failed.\n");
                continue;
            }

            printf("we got a new connection, client_count=%d, ip=%s, port=%d\n", 
            m_connfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            array_fd.push_back(m_connfd);

            if(m_connfd > max_fd)
            {
                max_fd = m_connfd;
            }
        }

        //遍历所有的客户端连接，找到发送数据的那个客户端描述符
        for(auto it = array_fd.begin(); it < array_fd.end(); it++)
        {
            if (FD_ISSET(*it, &tmpfd))
            {
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));

                int recv_len = -1;
                recv_len = recv(*it, buffer, sizeof(buffer), 0);

                if(recv_len == -1)
                {
                    printf("Receive data error.\n");
                    break;
                }
                else if(recv_len == 0)
                {
                    close(*it);
                    FD_CLR(*it, &tmpfd);
                    array_fd.erase(it);

                    printf("client_socket=[%d] close, ip=%s, port=%d\n\n", 
                    *it, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                }
                else
                {
                    buffer[recv_len] = '\0';
                    printf("server recv:%s\n", buffer);
                }
            }
        }
    }

    close(m_sockfd);
    return 0;
}