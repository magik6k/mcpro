#include "net/util.h"
#include "net/defines.h"
#include "client/client.h"

int clientEpoolFd;
struct epoll_event *clientEventBuf;

namespace mcpro
{
    namespace net
    {
        namespace core
        {
            int initRecvClient()
            {
                clientEpoolFd = epoll_create1 (0);
                if(clientEpoolFd == -1)return -1;

                struct epoll_event event;
                clientEventBuf = (epoll_event*)calloc (NET_MAXEVENTS, sizeof event);

                return 0;
            }

            int addClient(int fd, mcpro::client::client* c)
            {
                struct epoll_event event;
                event.data.fd = fd;
                event.data.ptr = c;
                event.events = EPOLLIN | EPOLLET;

                if(epoll_ctl (clientEpoolFd, EPOLL_CTL_ADD, fd, &event))
                {
                    printf("[Very error]Cannot add client socket to epool");
                    return -1;
                }
                return 0;
            }
        }
    }
}
