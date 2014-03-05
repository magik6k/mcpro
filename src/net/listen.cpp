#include "net/util.h"
#include "net/defines.h"
#include "net/core.h"

int serverFd;
int serverEpoolFd;
struct epoll_event *serverEventBuf;

namespace mcpro
{
    namespace net
    {
        namespace core
        {
            int startListening()
            {
                serverFd = mcpro::net::util::create_and_bind ("5555");
                if (serverFd == -1) return -1;
                if (mcpro::net::util::make_socket_non_blocking(serverFd) == -1)return -2;
                if (listen (serverFd, SOMAXCONN) == -1)return -3;


                serverEpoolFd = epoll_create1 (0);
                if(serverEpoolFd == -1)return -4;

                struct epoll_event event;
                event.data.fd = serverEpoolFd;
                event.events = EPOLLIN | EPOLLET;

                if(epoll_ctl (serverEpoolFd, EPOLL_CTL_ADD, serverFd, &event) == -1)return -5;
                serverEventBuf = (epoll_event*)calloc (NET_MAXEVENTS, sizeof event);

                while(true)
                {
                    int nevents = epoll_wait (serverEpoolFd, serverEventBuf, NET_MAXEVENTS, -1);
                    for(int eventId = 0; eventId < nevents; ++eventId)
                    {
                        if ((serverEventBuf[eventId].events & EPOLLERR) ||
                          (serverEventBuf[eventId].events & EPOLLHUP) ||
                          (!(serverEventBuf[eventId].events & EPOLLIN)))
                        {
                          printf("[Super-Critical error :(]Listener is dead!");
                          perror ("accept");
                          abort();
                        }
                        while(mcpro::net::core::acceptConnectoion(serverFd) > 0);
                    }
                }

                return 0;
            }
        }
    }
}
