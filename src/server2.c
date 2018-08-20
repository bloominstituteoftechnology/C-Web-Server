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

#define PORT "3490" // The port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

/**
 * Handle SIGCHILD signal
 *
 * We get this signal when a child process dies. This function wait()s for
 * Zombie processes.
 *
 * This is only necessary if we've implemented a multiprocessed version with
 * fork().
 */
void sigchild_handler(int s){
    (void)s; //quiet unused variable warning

    //waitpad() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    // Wait for all the children that have died, discard the exit status
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void start_reaper(void)
{
    struct sigaction sa;

    sa.sa_handler = sigchild_handler; // Reap allt dead processes
    sigempty(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // restart signal handler if interrupted
    if (sigaction(SIGCHILD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
