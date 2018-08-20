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
#define PORT "3490" // the local host users will connect to
#define BACKLOG 10 // capacity for pending connections queue

// handle SIGCHILD signal-We get this signal when a child process dies. This
// function waits() for Zombie processes. This is only necessary if we've 
// implemented a multiproccessed version with fork().
void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    // wait for all the children that have died, discard the exit status
    while(waitpid(-1, NULL, WNOHANG) > 0);
    // I believe this is errno is restored shold waitpid() overwrites it. 
    errno = saved_errno;
}

// Set up a signal handler that listens for child proccesses to die so
// they can be reaped with wait(). Whenever a child process dies, the parent
// process gets signal SIGCHILD; the handler sigchld_handler() takes care of
// wait()ing. This is only necessary if we've implemented a multiprocessed
// version with fork().
void start_reaper(void)
{
    struct sigaction sa;

    sa.sa_handler = sigchld_handler; // reap all dead processes.
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // restart signal handler if interrupted
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}

// This gets an Internet address, either IPv4 or IPv6.
// Helper function to make printing easier
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// returns the main listening socket, returns -1 or error
int get_listener_socket(char *port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int yes=1;
    int rv;

    // This code block lookds at the local network interfaces and
    // tries to find some that match our requirements(either IPv4 or
    // IPv6(AF_UNSPEC) and TCP(SOCK_STREAM) and use any IP on this
    // machine (AI_PASSIVE)).
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) !=0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_stderror(rv));
        return -1;
    }

    // Once we have a list of potention interfaces, loop through
    // them and try to set up a socket on each. Quit looping the 
    // first time we have success.
    for(p=servinfo; p!=NULL; p=p->ai_next)
    {
        // try and make a socket based on this canidate interface
        if ((sockfd=socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1)
        {
            continue;
        }
    

    // SO_REUSEADDR prevents the "address already in use" errors
    // that commonly come up when testing servers.
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1)
        {
            perror("setsockopt");
            close(sockfd);
            freeaddrinfo(servinfo); // all done with this structure
            return -2;
        }

    // See if we can bind this socket to this local IP address. This
    // associates the file descriptor(socket discritor) thjat we will
    // read and write on with a specific IP address.
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
        close(sockfd);
        continue;
    }

    // if we got here, we have a bound socket and we're done.
    break;
    }

    freeaddrinfo(servinfo); // all done with this structure.

    // if p is null, it means we didnt break out of our loop, and
    // therefor we have a bad socket.
    if (p == NULL)
    {
        fprintf(stderr, "webserver: failed to find local address\n");
        return -3;
    }

    // start listening. This is what allows remote computers to 
    // connect to this socket/IP
    if (listen(sockfd, BACKLOG) == -1)
    {
        close(sockfd);
        return -4;
    }

    return sockfd;
}

// Send an HTTP response that looks like this:
/*
 * header:       "HTTP/1.1 404 NOT FOUND" or "HTTP/1.1 200 OK", etc.
 * content_type: "text/plain", etc.
 * body:         the data to send.
*/
// return the value from the send() function.
int send_response(int fd, char *header, char *content_type, char *body)
{
    const int max_response_size = 65536;
    char response[max_response_size];
    int response_length; // total length of header+body;

    // !!!!!! IMPLEMENT ME !!!!!!!!!

    // send it all
    int rv = send(fd, response, response_length, 0);

    if(rv<0)
    {
        perror("send");
    }

    return rv;
}
