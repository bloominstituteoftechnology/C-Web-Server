/**
 * webserver.c -- A webserver written in C
 * 
 * Test with curl (if you don't have it, install it):
 * 
 *    curl -D - http://localhost:3490/
 *    curl -D - http://localhost:3490/d20
 *    curl -D - http://localhost:3490/date
 * 
 * You can also test the above URLs in your browser! They should work!
 * 
 * Posting Data:
 * 
 *    curl -D - -X POST -H 'Content-Type: text/plain' -d 'Hello, sample data!' http://localhost:3490/save
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

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

/**
 * Handle SIGCHILD signal
 *
 * We get this signal when a child process dies. This function wait()s
 * for Zombie processes.
 */
void sigchld_handler(int s) {
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    // Wait for all children that have died, discard the exit status
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/**
 * Set up a signal handler that listens for child processes to die so
 * they can be reaped with wait()
 *
 * Whenever a child process dies, the parent process gets signal
 * SIGCHLD; the handler sigchld_handler() takes care of wait()ing.
 */
void start_reaper(void)
{
    struct sigaction sa;

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
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
    int yes=1;
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

/**
 * Send an HTTP response
 * 
 * header is something like "HTTP/1.1 404 NOT FOUND" or "HTTP/1.1 200 OK"
 * content_type is "text/plain", etc.
 * body is the data to send
 * 
 * Return the value from the send() function.
 */
int send_response(int fd, char *header, char *content_type, char *body)
{
    char response[99999];
    int content_length = strlen(body);
    int response_length = sprintf(response,
        "%s\n"
        "Date: %s\n"
        "Connection: closed\n"
        "Content-Length: %d\n"
        "Content-Type: %s\n"
        "\n"
        "%s",

        header,
        "some day",
        content_length,
        content_type,
        body
    );

    int count = send(fd, response, response_length, 0);

    if (count < 0) {
        perror("send");
    }
}


/**
 * Send a 404 response
 */
void resp_404(int fd, char *path)
{
    char response_body[1024];

    sprintf(response_body, "404: %s not found", path);

    send_response(fd, "HTTP/1.1 404 NOT FOUND", "text/html", response_body);

}

/**
 * Send a / endpoint response
 */
void get_root(int fd)
{
    send_response(fd, "HTTP/1.1 200 OK", "text/html", "<h1>Hello Squeel!</h1>");
}

/**
 * Send a /d20 endpoint response
 */
void get_d20(int fd)
{
    srand(time(NULL));
    int r = rand() % 21; 
    char str[10];
    sprintf(str, "%d", r);

    send_response(fd, "HTTP/1.1 200 OK", "text/html", str);
}

/**
 * Send a /date endpoint response
 */
void get_date(int fd)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char str[200];

    sprintf(str, "%d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    send_response(fd, "HTTP/1.1 200 OK", "text/html", str);
}

/**
 * Post /save endpoint data
 */
void post_save(int fd, char *body)
{
    // Save the body and send a response
    char *status;

    // Open the file that we write the post data to
    int file_fd = open("data.txt", O_CREAT|O_WRONLY, 0644);

    // a file descriptor returns a negative number if there was an error opening it
    if (file_fd >= 0) {
        // lock the file so we properly update the file, without other processes reading from it
        // this prevents concurrency issues with multiple processes trying to write to the filea the same time
        flock(file_fd, LOCK_EX);

        // write the body message to the file
        write(file_fd, body, strlen(body));

        // unlock the file
        flock(file_fd, LOCK_UN);

        // close the file
        close(file_fd);

        // set the reponse status to ok
        status = "ok";
    } else {
        // set the reponse status to fail
        status = "fail";
    }

    // Now send an HTTP response

    char response_body[128];

    // print in in the status of the writing of to the file or not
    sprintf(response_body, "{\"status\": \"%s\"}", status);

    // send the json reponse of the status of success of the writing of the file
    send_response(fd, "HTTP/1.1 200 OK", "application/json", response_body);
}

/**
 * Search for the end of the HTTP header
 * 
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 */
char *find_end_of_header(char *header)
{
    // find the first occruence of '\r\n', '\r\r', '\n\n'
    char *p;

    p = strstr(header, "\n\n");

    if (p != NULL) return p;

    p = strstr(header, "\r\n\r\n");

    if (p != NULL) return p;

    p = strstr(header, "\r\r");

    return p;
}

/**
 * Handle HTTP request and send response
 */
void handle_http_request(int fd)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];
    char *p;
    char request_method[8]; // GET or POST
    char request_path[1024]; // /info etc.
    char request_protocol[128]; // HTTP/1.1

    // Read request and get the number of bytes in the request, returns an int
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0) {
        perror("recv");
        return;
    }

    // NUL terminate request string
    request[bytes_recvd] = '\0';

    // printf("%s\n", request);

    // Parse the first line of the request which has method, path, and protocol type 
    char *first_line = request;

    // Look for newline
    p = strchr(first_line, '\n');
    *p = '\0';

    // Remaining header
    char *header = p + 1; // +1 to skip the '\n'

    // Look for two newlines marking the end of the header
    p = find_end_of_header(header);

    // p should be two new lines and print out two lines of nothing
    printf("%s\n", p);

    if (p == NULL) {
        printf("Could not find end of header\n");
        exit(1);
    }

    // And here is the body
    char *body = p;

    // read the headers
    sscanf(request, "%s %s %s", request_method, request_path, request_protocol);

    // printf("Method %s\n", request_method);
    // printf("Path %s\n", request_path);

    if (strcmp(request_method, "GET") == 0) {
        if (strcmp(request_path, "/") == 0) {
            get_root(fd);
        }
        if (strcmp(request_path, "/d20") == 0) {
            get_d20(fd);
        }
        if (strcmp(request_path, "/date") == 0) {
            get_date(fd);
        }
    }
    else if (strcmp(request_method, "POST") == 0) {
        if (strcmp(request_path, "/save") == 0) {
            // get the body data
            post_save(fd, body);
        }
    }

    // !!!! IMPLEMENT ME

    // Parse the header
    // Get the request type and path from the first line
    // find_end_of_header()
    // call the appropriate handler functions, above, with the incoming data
}

/**
 * Main
 */
int main(void)
{
    int newfd;  // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];

    // Start reaping child processes
    start_reaper();

    // Get a listening socket
    int listenfd = get_listener_socket(PORT);

    if (listenfd < 0) {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }

    printf("webserver: waiting for connections...\n");

    // This is the main loop that accepts incoming connections and
    // fork()s a handler process to take care of it. The main parent
    // process then goes back to waiting for new connections.
    
    while(1) {
        socklen_t sin_size = sizeof their_addr;

        // Parent process will block on the accept() call until someone
        // makes a new connection:
        newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }

        // Print out a message that we got the connection
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        
        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.

        if (fork() == 0) {
            // We're the child process

            // We don't need the listening socket. The parent
            // process's listenfd is still open--we just close it in the
            // child.
            close(listenfd);

            // This does the heavy lifting, recv() the HTTP request and
            // send() the HTTP response.
            handle_http_request(newfd);

            // And this child is done! Bye bye!
            exit(0);
        }

        // Parent process out here, still

        // Parent doesn't need this. We need to close them as we get
        // them so we don't fill up the parent's file descriptor table.
        // The child's copy of newfd remains open.
        close(newfd);
    }

    // Unreachable code

    return 0;
}

