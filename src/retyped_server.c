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