#include "net/util.h"
#include "net/core.h"
#include "client/client.h"

namespace mcpro
{
    namespace net
    {
        namespace core
        {
            int acceptConnectoion(int serverFd)
            {
                struct sockaddr in_addr;
                socklen_t in_len;
                int clientFd;
                char hostbuf[NI_MAXHOST], portbuf[NI_MAXSERV];

                in_len = sizeof in_addr;
                clientFd = accept (serverFd, &in_addr, &in_len);

                if(clientFd == -1)
                {
                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                    {
                        return 0;///no more connections to accept
                    }
                    else
                    {
                        printf("[Error :D]net/accept.cpp: accept error");
                        perror ("accept");
                        return -1;
                    }
                }

                ///Print some info, I like info

                if(getnameinfo (&in_addr, in_len, hostbuf, sizeof hostbuf, portbuf, sizeof portbuf, NI_NUMERICHOST | NI_NUMERICSERV) == 0)
                    printf("Accepted connection on descriptor %d (host=%s, port=%s)\nConnecting to backend..", clientFd, hostbuf, portbuf);

                if(mcpro::net::util::make_socket_non_blocking(clientFd) == -1)
                {
                    printf("[Critical Error xD]net/accept.cpp: cannot make client socket non-blocking");
                    return 1;
                }

                int backendfd = mcpro::net::util::create_and_connect("rs.meetnet.pl","33333");

                if(backendfd == -1)return 1;

                if(mcpro::net::util::make_socket_non_blocking(backendfd) == -1)
                {
                    printf("[Critical Error xD]net/accept.cpp: cannot make backend socket non-blocking");
                    return 1;
                }

                printf("Connected to backend");

                mcpro::client::client* c = new mcpro::client::client(clientFd, backendfd);

                mcpro::net::core::addClient(clientFd, c);
                mcpro::net::core::addBackend(backendfd, c);

                return 1;
            }
        }
    }
}
