#pragma once
#include "client/client.h"

namespace mcpro
{
    namespace net
    {
        namespace core
        {
            int startListening();
            int acceptConnectoion(int serverFd);

            int addClient(int fd, mcpro::client::client* c);
            int addBackend(int fd, mcpro::client::client* c);
        }
    }
}
