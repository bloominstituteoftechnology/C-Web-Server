/**
 * webserver.c -- A webserver written in C
 * 
 * Test with curl (if you don't have it, install it):
 * 
 * curl -D - http://localhost:3490/
 * curl -D - http://localhost:3490/d20
 * curl -D - http://localhost:3490/date
 * 
 * You can also test the above URLs in your browser! They should work!
 * 
 * Posting Date:
 * 
 *  curl -D - -X POST -H 'Content-Type: text/plain' -d 'Hello, sample data!' http://localhost:3490/save 
 * 
 * (Posting data is harder to test from a browser.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>

#define PORT "3490" // the port users will be connecting to

#define BACKLOG 10  // how many pending connections queue will hold

/**
 * Handle SIGCHILD signal
 * 
 * We get this signal when a child process dies. This function wait()s for 
 * Zombie processes.
 * 
 * This is only necessary if we've implemented a multiprocessed version with 
 * fork().
 */
void sigchild_handler(int s) {
    (void)s;    // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    // wait for all children that have died, discard the exit status
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/**
 * Set up a signal handler that listens for child processes to die so 
 * they can be reaped with wait()
 * 
 * Whenever a child process dies, the parent process gets signal 
 * SIGCHILD; the handler sigchild_handler() takes care of wait()ing.
 * 
 * This is only necessary is we've implemented a multiprocessed version with 
 * fork().
 */
void start_reaper(void)
{
    struct sigaction sa;

    sa.sa_handler = sigchild.handler; // Reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;   //Restart signal handler if interrupted
    if (sigaction(SIGCHILD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

/**
 * This gets an Intert address, either IPv4 or IPv6
 * 
 * Helper function to make printing easier.
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return & (((struct sockaddr_in*)sa)->sin_addr);
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
    int yes=1;
    int rv;

    // This block of code looks at the local network interfaces and 
    // tries to find some that match our requirements (namely either)
    // IPv4 or IPv6 (AF_UNSPEC) and TCP (SOCK_STREAM) and use any IP on
    // this machine (AI_PASSIVE). 

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) !=0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // Once we have a list of potential interfaces, loop through them
    // and try to set up  

}

