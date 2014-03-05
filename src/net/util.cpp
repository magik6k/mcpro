#include "net/util.h"
/** Credit to http://kovyrin.net/2006/04/13/epoll-asynchronous-network-programming/ */


namespace mcpro
{
    namespace net
    {
        namespace util
        {

            int create_and_bind (const char *port)
            {
              struct addrinfo hints;
              struct addrinfo *result, *rp;
              int s, sfd;

              memset (&hints, 0, sizeof (struct addrinfo));
              hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
              hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
              hints.ai_flags = AI_PASSIVE;     /* All interfaces */

              s = getaddrinfo (NULL, port, &hints, &result);
              if (s != 0)
                {
                  fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
                  return -1;
                }

              for (rp = result; rp != NULL; rp = rp->ai_next)
                {
                  sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
                  if (sfd == -1)
                    continue;

                  s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
                  if (s == 0)
                    {
                      /* We managed to bind successfully! */
                      break;
                    }

                  close (sfd);
                }

              if (rp == NULL)
                {
                  fprintf (stderr, "Could not bind\n");
                  return -1;
                }

              freeaddrinfo (result);

              return sfd;
            }

            int create_and_connect(const char* host, const char* port)
            {
                struct addrinfo host_info;
                struct addrinfo *host_info_list;
                int sfd;

                memset(&host_info, 0, sizeof host_info);
                host_info.ai_family = AF_UNSPEC;
                host_info.ai_socktype = SOCK_STREAM;

                if((sfd = getaddrinfo(host, port, &host_info, &host_info_list)) != 0)
                {
                    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (sfd));
                    return -1;
                }

                sfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);

                if (sfd == -1)
                {
                    printf ("[Error]Cannot create socket to backend");
                    return -1;
                }

                if(connect(sfd, host_info_list->ai_addr, host_info_list->ai_addrlen) == -1)
                {
                    printf ("[Error]Cannot connect to backend");
                    return -1;
                }

                return sfd;
            }

            int make_socket_non_blocking (int sfd)
            {
              int flags, s;

              flags = fcntl (sfd, F_GETFL, 0);
              if (flags == -1)
                {
                  perror ("fcntl");
                  return -1;
                }

              flags |= O_NONBLOCK;
              s = fcntl (sfd, F_SETFL, flags);
              if (s == -1)
                {
                  perror ("fcntl");
                  return -1;
                }

              return 0;
            }
        }
    }
}
