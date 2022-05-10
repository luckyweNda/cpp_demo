#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define OPEN_MAX 5000

int main()
{
    sockaddr_in srv_addr, clt_addr;
    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(51000);

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(lfd, (sockaddr*)&srv_addr, sizeof(srv_addr));
    listen(lfd, 128);

    int efd = epoll_create(OPEN_MAX);
    epoll_event tep, ep[OPEN_MAX];

    tep.events = EPOLLIN;
    tep.data.fd = lfd;

    int res = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &tep);

    while(true)
    {
        int nready = epoll_wait(efd, ep, OPEN_MAX, -1);
        if(nready < 0)
        {
            printf("epoll wait error.\n");
            return 0;
        }

        for(int i = 0; i < nready; i++)
        {
            if(!(ep[i].events & EPOLLIN))
                continue;

            if(ep[i].data.fd == lfd)
            {
                int temp = sizeof(clt_addr);
                int cfd = accept(lfd, (sockaddr*)&clt_addr, (socklen_t*)&temp);
                
                char buf[256];
                memset(buf, 0, sizeof(buf));

                std::cout 
                << "Client connect: "
                << inet_ntop(AF_INET, &clt_addr.sin_addr.s_addr, buf, sizeof(buf))
                << ": "
                << ntohs(clt_addr.sin_port)
                << std::endl;

                tep.events = EPOLLIN;
                tep.data.fd = cfd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &tep);

                if(res < 0)
                {
                    printf("epoll_ctl error.\n");
                    return 0;
                }
            }
            else 
            {
                int sockfd = ep[i].data.fd;
                char buf[256];
                memset(buf, 0, sizeof(buf));
                recv(sockfd, buf, sizeof(buf), 0);
                int ret = strlen(buf);

                if(ret < 0)
                {
                    if(errno == ECONNRESET)
                    {
                        std::cout << "Connection has been reset.\n";
                        res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);

                        if(res < 0)
                        {
                            printf("epoll_ctl error.\n");
                            return 0;
                        }

                        close(sockfd);
                    }
                    else
                    {
                        printf("read error.\n");
                        return 0;
                    }
                }
                else if(ret == 0)
                {
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                    if(res < 0)
                    {
                        printf("epoll_ctl error.\n");
                        return 0;
                    }

                    close(sockfd);
                    std::cout 
                    << "Client disconnect: "
                    << inet_ntop(AF_INET, &clt_addr.sin_addr.s_addr, buf, sizeof(buf))
                    << ": "
                    << ntohs(clt_addr.sin_port)
                    << std::endl;
                }
                else 
                {
                    printf("Receive a message from client:\n%s", buf);
                }
            }
        }
    }

    close(lfd);
    return 0;
}