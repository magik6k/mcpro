#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

namespace mcpro
{
    namespace net
    {
        namespace util
        {
            int create_and_bind (const char *port);
            int make_socket_non_blocking (int sfd);
            int create_and_connect(const char* host, const char* port);
        }
    }
}
