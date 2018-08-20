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