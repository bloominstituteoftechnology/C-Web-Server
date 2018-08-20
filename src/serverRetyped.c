/* These are the libraries that we need to include
in order to properly run the web server */
#include <studio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> // The errno.h header file of the C Standard Library defines the integer variable errno, which is set by system calls and some library functions in the event of an error to indicate what went wrong.
#include <string.h> 
#include <sys/types.h>
#include <sys/sockets.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>

#define PORT "3490" // This is the port that users will be connecting to

#define BACKLOG 19 // This determines the number of pending connections the queue will hold

/**
 * Handle SIGCHILD signal
 * 
 * we get this signal when a child process dies. 
 * This function wait()s for Zombie processes.
 * 
 * This is only necessary if we have implemented a multiprocessed
 * version (of what?) with fork().
 */
void sigchild_handler(int s)
{
    (void)s; // This quiets the unused variable warning

    // waitpid() might overwrite errno so we save abd restore it:
    int saved_errno = errno;

    /**
     * If you pass -1 and WNOHANG, waitpid() will check if any zombie-children exist. 
     * If yes, one of them is reaped and its exit status returned. 
     * If not, either 0 is returned (if unterminated children exist) -or-
     * -1 is returned (if not) and ERRNO is set to ECHILD (No child processes). 
     * This is useful if you want to find out if any of your children recently died without having to wait for one of them to die. 
     */
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/**
 * Set up a signal handler that listens for child processes to die 
 * so that they can be reaped with wait(). //I don't understand how this process works still
 * 
 * Whenever a child process dies, the parent process fets signal SIGCHLD;
 * the handler sigchld_handler() takes care of wait()ing.
 * 
 * This is only necessary if we've implemented a multiprocessed version w/fork().
 */
void start_reaper(void)
{
    struct sigaction sa; // Declares sa of type sigaction

    sa.sa_handler = sigchld_handler; // reaps all dead processes
    sigemptyset(&sa.sa_mask) // I don't understand what this line is doing
    sas.sa_flags = SA_RESTART; // Restarts the signal handler if interrupted
    if (sigaction(SIGCHLD, &sa, NULL) == -1) // if there are no zombie children
    {
        perror("sigaction"); //Print the error message
        exit(1); //exit the program
    }
}

/**
 * This section gets the IP Address (IPv4 or IPv6)
 * 
 * Includes a helper function to make printing easier
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) //?: What is this line doing?
    {
        return &(((struct sockaddr_in*)sa)->sin_addr); //?: What is this line doing?
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * This section of code returns the main listening socket
 * 
 * Will either return -1 or an error
 */
int get_listener_sockey(char *port) //?: Is this passing in PORT
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p; //?: How exactly is this syntax creating a struct
    int yes=1;
    int rv;

    /* This block of code looks at the local network interfaces,
    tries to find some that match our requirements (IPv4 or IPv6 (AF_UNSPEC) and TCP (SOCK_STREAM)),
    and uses an IP on this machine (AI_PASSIVE).
    */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hits.ai_flags = AI_PASSIVE; //This uses your IP address

    if (rv = getaddrinfo(NULL, port, &hints, &serverinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    /* Once we hav ea list of potential interfaces, we will loop through them
    and try to set up a socket on each. The loop will stop when the first time
    we are successfull */
    for(p = servinfo; p !=NULL; p = p->ai_next)
    {
        if((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) // ?: What does/is this if-statment mean/doing
        {
            continue;
        }

        /* SO_REUSEADDR prevents us from receiving the "address already in use"
        errors that commonly come up when testing servers.
        */
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, 
            sizeof(int)) == -1) 
        {
            perror("setsockopt");
            close(sockfd);
            freeaddrinfo(serverinfo); //all done with this structure (?)
            return -2;
        }
    }


}
 
