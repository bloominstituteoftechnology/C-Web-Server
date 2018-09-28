#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "net.h"

#define BACKLOG 10	 // how many pending connections queue will hold

/**
 * This gets an Internet address, either IPv4 or IPv6
 *
 * Helper function to make printing easier.
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * Return the main listening socket
 *
 * Returns -1 or error
 */
int get_listener_socket(char *port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;
    int rv;

    // This block of code looks at the local network interfaces and
    // tries to find some that match our requirements (namely either
    // IPv4 or IPv6 (AF_UNSPEC) and TCP (SOCK_STREAM) and use any IP on
    // this machine (AI_PASSIVE).

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // Once we have a list of potential interfaces, loop through them
    // and try to set up a socket on each. Quit looping the first time
    // we have success.
    for(p = servinfo; p != NULL; p = p->ai_next) {

        // Try to make a socket based on this candidate interface
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
            //perror("server: socket");
            continue;
        }

        // SO_REUSEADDR prevents the "address already in use" errors
        // that commonly come up when testing servers.
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
            sizeof(int)) == -1) {
            perror("setsockopt");
            close(sockfd);
            freeaddrinfo(servinfo); // all done with this structure
            return -2;
        }

        // See if we can bind this socket to this local IP address. This
        // associates the file descriptor (the socket descriptor) that
        // we will read and write on with a specific IP address.
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            //perror("server: bind");
            continue;
        }

        // If we got here, we got a bound socket and we're done
        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    // If p is NULL, it means we didn't break out of the loop, above,
    // and we don't have a good socket.
    if (p == NULL)  {
        fprintf(stderr, "webserver: failed to find local address\n");
        return -3;
    }

    // Start listening. This is what allows remote computers to connect
    // to this socket/IP.
    if (listen(sockfd, BACKLOG) == -1) {
        //perror("listen");
        close(sockfd);
        return -4;
    }

    return sockfd;
}
