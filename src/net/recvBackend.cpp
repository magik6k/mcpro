#include "net/util.h"
#include "net/defines.h"
#include "client/client.h"

int backendEpoolFd;
struct epoll_event *backendEventBuf;

namespace mcpro
{
    namespace net
    {
        namespace core
        {
            int initRecvBackend()
            {
                backendEpoolFd = epoll_create1 (0);
                if(backendEpoolFd == -1)return -1;

                struct epoll_event event;
                backendEventBuf = (epoll_event*)calloc (NET_MAXEVENTS, sizeof event);

                return 0;
            }

            int addBackend(int fd, mcpro::client::client* c)
            {
                struct epoll_event event;
                event.data.fd = fd;
                event.data.ptr = c;
                event.events = EPOLLIN | EPOLLET;

                if(epoll_ctl (backendEpoolFd, EPOLL_CTL_ADD, fd, &event))
                {
                    printf("[Very error]Cannot add client socket to epool");
                    return -1;
                }
                return 0;
            }
        }
    }
}
