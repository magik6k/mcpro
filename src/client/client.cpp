#include "client/client.h"

namespace mcpro
{
    namespace client
    {
        client::client(int clientfd, int backendfd)
        {
            this->cfd = clientfd;
            this->bfd = backendfd;
        }
    }
}
