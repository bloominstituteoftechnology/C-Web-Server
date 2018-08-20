#include <stdio.h> // standard input/output
#include <stdlib.h> // standard library
#include <unistd.h> // standard symbolic constants and types
#include <errno.h> // system error numbers
#include <string.h> // string operations
#include <sys/types.h> // data types
#include <sys/socket.h> // makes available a type, socklen_t, which is an 
// unsigned opaque integral type of length of at least 32 bits. To forestall 
// portability problems, it is recommended that applications should not use 
// values larger than 232 - 1.
#include <netinet/in.h> // defines the sockaddr_in structure
#include <netdb.h> // definitions for network database operations
#include <arpa/inet.h> // definitions for internet operations
#include <sys/wait.h> // waitpid()
#include <signal.h> //  defines symbolic constants, each of which 
// expands to a distinct constant expression of the type
#include <time.h> // declares the structure tm, that has subtypes like
// seconds, minutes, hours, days, months, years, etc.
#include <sys/file.h> // TODO: not sure what this is.
#include <fcntl.h> // file control options
#define PORT "3490" // the local host users will connect to
#define BACKLOG 10 // capacity for pending connections queue

// handle SIGCHILD signal-We get this signal when a child process dies. This
// function waits() for Zombie processes. This is only necessary if we've 
// implemented a multiproccessed version with fork().
void sigchld_handler(int s)
{
    // quiet unused variable warning
    (void)s; 
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

// send a 404
void resp_404(int fd)
{
  send_response(fd, "HTTP/1.1 404 NOT FOUND", "text/html", "<h1>404 Page Not Found</h1>");
}

// TODO: send a / endpoint response
void get_root(int fd)
{
    // !!!!! implement me!!!!!
}

// TODO: send a /d20 endpoint response
void get_d20(int fd)
{
    // 20-sided die RNG COMING SOON
}

// TODO: send a /date endpoint response
void get_date(int fd)
{
    // !!!! implement me
}

// TODO: post /save enpoint data
void post_save(int fd, char *body)
{
    // save the body and send a response
}

// TODO: search for the start of the HTTP body. The body is after
// the header, seperated from it by a blank line,(2 newlines in a row).
// "newlines" om HTTP can be \r\n (carriage return followed by newline) 
// or \n (newline) or \r (carriage return)
char *find_start_of_body(char *header)
{
    //!!!!!implement me!!!!!!!
}

// Handle HTTP request and send response
void handle_http_request(int fd)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];
    char *p;
    char request_type[8]; // GET or POST
    char request_path[1024]; // /info etc.
    char request_protocol[128]; // HTTP/1.1

    // read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if(bytes_recvd < 0)
    {
        perror("recv");
        return;
    }

    // NULL terminate request string
    request[bytes_recvd] = '\0';

  // !!!! IMPLEMENT ME
  // Get the request type and path from the first line
  // Hint: sscanf()!

  // !!!! IMPLEMENT ME (stretch goal)
  // find_start_of_body()

  // !!!! IMPLEMENT ME
  // call the appropriate handler functions, above, with the incoming data
}

int main(void)
{
    int newfd; // listen on sock_fd, new connections on newfd
    struct sockaddr_storage their_addr; // connector's address info
    char s[INET6_ADDRSTRLEN];

    // start reaping child processes
    start_reaper();

    // get a listening socket
    int listenfd = get_listener_socket(PORT);

    if(listenfd < 0)
    {
        fprintf(stderr, "webserver: fatal error gettting listenign socket\n");
        exit(1);
    }

    printf("webserver: waiting for connections...");

    // this is the main loop that accepts incoming connetions and
    // fork()s a handler process to take care of it. Tthe main parent
    // process then goes back to waiting for new connections. 

    while(1)
    {
        socklen_t sin_size = sizeof their_addr;

        // parent process will block on the accept() call until someone
        // makes a new connection.
        newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.

        // !!!!! IMPPLMENT me! (stretch)-convert this to be multiprocessed with fork()

        handle_http_request(newfd);

        // done with this
        close(newfd);
    }
    //unreachable code
    return 0;
}
