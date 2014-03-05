#pragma once
#include "client/clientState.h"

namespace mcpro
{
    namespace client
    {
        class client
        {
            private:
                int cfd;
                int bfd;
                mcpro::client::clinetState state;
            public:
                client(int clientfd, int backendfd);

        };
    }
}
