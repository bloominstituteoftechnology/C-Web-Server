#include <stdio.h>    //Enables the use of input and output functions
#include <stdlib.h>   // Enables the use of several general purpose functions, including dynamic memory management, random number generation, communication with the environment, integer arithmetics, searching, sorting and converting.
#include <unistd.h> //Header defines miscellaneous symbolic constants and types, and declares miscellaneous functions
#include <errno.h>//Header defines many system error
#include <string.h>// Header defines one variable type, one macro, and various functions for manipulating arrays of characters.
#include <sys/types.h>// Header includes many defininations for data types that are used related to the directory
#include <sys/socket.h>//Header defines Internet Protocols
#include <netinet/in.h>//Header defines Internet Protocols related to ports
#include <netdb.h>// Definitions for network database operations
#include <arpa/inet.h>//Definitions for internet operations
#include <sys/wait.h>//Declarations for waitin
#include <signal.h>//Signals? TODO learn what this does
#include <time.h>//Define Time type
#include <sys/file.h>//Cant find any good defines or even whats included in this library but I would assume it defines file/os calls TODO research more about this library
#include <fcntl.h>//Defines file control options

#define PORT "3490" //Port that the users will connect too

#define BACKLOG 10//How many connections that will be hold in the queue

void sigchld_handler(int s)//This is a reaper for the child process, it signals when a child process termanates.
{
    (void)s; //Quiet unused variable warning

    int saved_errno = errno; //Waitpid() might overwrite errno, so we save and restore it:

    while (waitpid(-1, NULL, WNOHANG) > 0);
    // Wait for all children that have died, discard the exit status
    errno = saved_errno;
}

/*
 * Whenever a child process dies, the parent process gets signal
 * SIGCHLD; the handler sigchld_handler() takes care of wait()ing.
 * This is only necessary if we've implemented a multiprocessed version with
 * fork().
 */
void start_reaper(void)
{
    struct sigaction sa;

    sa.sa_handler = sigchld_handler;
    // Reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    // Restart signal handler if interrupted
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}

/**
 * This gets an Internet address, either IPv4 or IPv6
 *
 * Helper function to make printing easier.
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
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
    //This block of code is looking at the local network and trying to match our requirements(IPv4 or IPv6 and use the IP address on this machine)
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;//Local IP Addess

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }
    //Looping through the list of interfaces trying to set a socket for each one. Quit looping on the first success
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        // Try to make a socket based on this candidate interface
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            continue;
        }
        // SO_REUSEADDR prevents the "address already in use" errors
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            close(sockfd);
            freeaddrinfo(servinfo);//Freeing the memory
            return -2;
        }
        // See if we can bind this socket to this local IP address. This
        // associates the file descriptor (the socket descriptor) that
        // we will read and write on with a specific IP address.
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
             //perror("server: bind");
            continue;
        }
         // If we got here, we got a bound socket and we're done
        break;
    }
    freeaddrinfo(servinfo); //Free from memory
    //If NULL the loop didnt break and we had no sussessful sockets
    if (p == NULL)
    {
        fprintf(stderr, "webserver: failed to find local address\n");
        return -3;
    }
    // Start listening. This is what allows remote computers to connect
  // to this socket/IP.
    if (listen(sockfd, BACKLOG) == -1)
    {
        close(sockfd);
        return -4;
    }

    return sockfd;
}

int send_responese(int fd, char *header, char *content_type, char body)
{
    const int max_response_size = 65536;
    char response[max_response_size];

    time_t t1 = time(NULL);
    struct tm *ltime = localtime(&t1);

    int content_length = strlen(body);

    int response_length = sprintf(response, "%s\n"
                                            "Date: %s"
                                            "Connection: close\n"
                                            "Content-Length: %d\n"
                                            "Content-Type: %s\n"
                                            "\n"
                                            "%s\n",

                                  header,
                                  asctime(ltime),
                                  content_length,
                                  content_type,
                                  body);

    int rv = send(fd, response, response_length, 0);

    if (rv < 0)
    {
        perror("send");
    }
    return rv;
}

void resp_404(int fd, char *path)
{
    char response_body[1024];

    sprintf(response_body, "404: %s not found", path);

    send_responese(fd, "HTTP/1.1 404 NOT FOUND", "text/html", response_body);
}

void get_root(int fd)
{
    char *response_body = "<html><head></head><body><h1>Hello, World!</h1></body></html>\n";

    send_responese(fd, "HTTP/1.1 200 OK", "text/html", response_body);
}

void get_d20(int fd)
{
    srand(time(NULL) + getpid());

    char response_body[8];
    sprintf(response_body, "%d\n", (rand() % 20) + 1);

    send_responese(fd, "HTTP/1.1 200 OK", "text/plain", response_body);
}

void get_date(int fd)
{
    char response_body[128];
    time_t t1 = time(NULL);
    struct tm *gtime = gmtime(&t1);

    sprintf(response_body, "%s", asctime(gtime));

    send_responese(fd, "HTTP/1.1 200 OK", "text/plain", response_body);
}

void post_save(int fd, char *body)
{
    char *ststus;

    int file_fd = open("data.txt", O_CREAT | O_WRONLY, 0644);

    if (file_fd >= 0)
    {
        flock(file_fd, LOCK_EX);
        write(file_fd, body, strlen(body));
        flock(file_fd, LOCK_UN);
        close(file_fd);

        status = "ok";
    }
    else
    {
        status = "fail";
    }
    char response_body[128];

    sprintf(response_body, "{\"status\": \"%s\"}\n", status);

    send_responese(fd, "HTTP/1.1 200 OK", "application/json", response_body);
}

char *find_start_of_body(char *header)
{
    char *p;
    p = strstr(header, "\n\n");
    if (p != NULL)
        return p;
    p = strstr(header, "\r\n\r\n");
    if (p != NULL)
        return p;
    p = strstr(header, "\r\r");

    return p;
}

void handle_http_request(int fd)
{
    const int request_buffer_size = 65536;
    char request[request_buffer_size];
    char *p;
    char request_type[8];
    char request_type[1024];
    char request_type[128];

    int bytes_recvd = recv(fd, request, request_buffer_size -1, 0);

    if (bytes_recvd < 0){
        perror("recv");
        return;
    }
    request[bytes_recvd] = "\0";

    p = find_start_of_body(request);

    if (p == NULL){
        printf("Could not find end of header\n");
        exit(1);
    }
    char *body = p;

    printf("REQUEST: %s %s %s\n", request_type, request_path, request_protocol);

  if (strcmp(request_type, "GET") == 0) {

    if (strcmp(request_path, "/") == 0) {
      get_root(fd);
    }
    else if (strcmp(request_path, "/d20") == 0) {
      get_d20(fd);
    }
    else if (strcmp(request_path, "/date") == 0) {
      get_date(fd);
    }

    else {
      resp_404(fd, request_path);
    }
  }

  else if (strcmp(request_type, "POST") == 0) {

    if (strcmp(request_path, "/save") == 0) {
      post_save(fd, body);

    } else {
      resp_404(fd, request_path);
    }
  }

  else {
    fprintf(stderr, "unknown request type \"%s\"\n", request_type);
    return;
  }
}

int main(void)
{
    int newfd;
    struct sockaddr_storage their_addr;
    char s[INET6_ADDRSTRLEN];

    start_reaper();
    int listenfd = get_listener_socket(PORT);

    if(listenfd < 0){
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }
    printf("wenserver: waiting for connectins...\n");

    while(1){
        socklen_t sin_size = sizeof their_addr;
        newfd = accept(listenfd, (struct sockaddr *)& their_addr, &sin_size);
        if(newfd == -1){
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        if(fork()== 0){
            close(listenfd);
            handle_http_request(newfd);
            exit(0);
        }
        close(newfd);
    }
    return 0;
}