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
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>
#include "net.h"
#include "file.h"
#include "mime.h"
#include "cache.h"

#define PORT "3490"  // the port users will be connecting to

#define SERVER_FILES "./serverfiles"
#define SERVER_ROOT "./serverroot"

/**
 * Send an HTTP response
 *
 * header:       "HTTP/1.1 404 NOT FOUND" or "HTTP/1.1 200 OK", etc.
 * content_type: "text/plain", etc.
 * body:         the data to send.
 * 
 * Return the value from the send() function.
 */
//int send_response(int fd, char *header, char *content_type, void *body, int content_length)
int send_response(int fd, char *header, char *content_type, char *body)
{
  const int max_response_size = 65536;
  char response[max_response_size];
  int response_length; // Total length of header plus body

  // !!!!  IMPLEMENT ME
  int content_length = strlen(body);
  time_t timer = time(NULL);
  struct tm *date = localtime(&timer);

  response_length = sprintf(response, "%s\n Date: %s Connection: close\n Content-Length: %d\n Content-Type: %s\n\n %s\n",
                            header, asctime(date), content_length, content_type, body);

  // Send it all!
  int rv = send(fd, response, response_length, 0);

  if (rv < 0)
  {
    perror("send");
  }

  return rv;
}

/**
 * Send a 404 response
 */
void resp_404(int fd)
{
  send_response(fd, "HTTP/1.1 404 NOT FOUND", "text/html", "<h1>404 Page Not Found</h1>");
}

/**
 * Send a / endpoint response
 */
void get_root(int fd)
{
  // !!!! IMPLEMENT ME
  //send_response(...
  // char *response_body[1024];
  // sprintf(response_body, "<!DOCTYPE html><html><head><title>Lambda School</title></head><body><h1>Hello World!</h1></body></html>");
  char *response_body = "<!DOCTYPE html><html><head><title>Lambda School</title></head><body><h1>Crap Hello World!</h1></body></html>";
  send_response(fd, "HTTP/1.1 200 OK", "text/html", response_body);
}

/**
 * Send a /d20 endpoint response
 */
void get_d20(int fd)
{
  // !!!! IMPLEMENT ME
  char response_body[8];
  int randNum;

  srand(time(NULL) + getpid());

  randNum = (rand() % 20) + 1;
  sprintf(response_body, "%d\n", randNum);
  send_response(fd, "HTTP/1.1 200 OK", "text/plain", response_body);
}

/**
 * Send a /date endpoint response
 */
void get_date(int fd)
{
  // !!!! IMPLEMENT ME
  char response_body[1024];
  time_t timer = time(NULL);
  struct tm *date = gmtime(&timer);

  sprintf(response_body, "%s\n", asctime(date));
  send_response(fd, "HTTP/1.1 200 OK", "text/plain", response_body);
}

/**
 * Post /save endpoint data
 */
void post_save(int fd, char *body)
{
  // !!!! IMPLEMENT ME

  // Save the body and send a response
}

/**
 * Search for the start of the HTTP body.
 *
 * The body is after the header, separated from it by a blank line (two newlines
 * in a row).
 *
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 */
char *find_start_of_body(char *header)
{
  // !!!! IMPLEMENT ME
}

/**
 * Handle HTTP request and send response
 */
void handle_http_request(int fd)
{
  const int request_buffer_size = 65536; // 64K
  char request[request_buffer_size];
  char *p;
  char request_type[8];       // GET or POST
  char request_path[1024];    // /info etc.
  char request_protocol[128]; // HTTP/1.1

  // Read request
  int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

  if (bytes_recvd < 0)
  {
    perror("recv");
    return;
  }

  // NUL terminate request string
  request[bytes_recvd] = '\0';

  // !!!! IMPLEMENT ME
  // Get the request type and path from the first line
  // Hint: sscanf()!
  sscanf(request, "%s %s %s", request_type, request_path, request_protocol);

  // !!!! IMPLEMENT ME (stretch goal)
  // find_start_of_body()

  // !!!! IMPLEMENT ME
  // call the appropriate handler functions, above, with the incoming data
  if (strcmp(request_type, "GET") == 0)
  {
    if (strcmp(request_path, "/root") == 0)
    {
      get_root(fd);
    }
    else if (strcmp(request_path, "/d20") == 0)
    {
      get_d20(fd);
    }
    else if (strcmp(request_path, "/date") == 0)
    {
      get_date(fd);
    }
    else
    {
      resp_404(fd);
    }
  }
  else if (strcmp(request_type, "POST") == 0)
  {
    if (strcmp(request_path, "/save") == 0)
    {
      post_save(fd, p);
    }
    else
    {
      resp_404(fd);
    }
  }
}


/**
 * Main
 */
int main(void)
{
    int newfd;  // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];

    struct cache *cache = cache_create(10, 0);

    // Get a listening socket
    int listenfd = get_listener_socket(PORT);

    if (listenfd < 0) {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }

    printf("webserver: waiting for connections on port %s...\n", PORT);

    // This is the main loop that accepts incoming connections and
    // forks a handler process to take care of it. The main parent
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

        handle_http_request(newfd);

        close(newfd);
    }

    // Unreachable code

    return 0;
}

