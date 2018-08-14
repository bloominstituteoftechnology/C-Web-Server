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

#define PORT "3490"
#define BACKLOG 10
#define POST_FILENAME "data.txt"
#define REQ_BUFFER_SIZE 65536

/**
 * Handle SIGCHILD signal
 *
 * We get this signal when a child process dies. This function wait()s for
 * Zombie processes.
 */
void sigchld_handler(int s)
{
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

  sa.sa_handler = sigchld_handler; // Reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART; // Restart signal handler if interrupted
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
 * Return the value from the send() function.
 */
int send_response(int fd, char *header, char *content_type, char *body)
{
  time_t t = time(NULL);
  char response[REQ_BUFFER_SIZE];

  sprintf(
      response,
      "%s\nDate: %sConnection: close\nContent-Length: %ld\nContent-Type: %s\n\n%s",
      header,
      asctime(localtime(&t)),
      strlen(body),
      content_type,
      body);

  int response_length = strlen(response);
  int rv = send(fd, response, response_length, 0);

  if (rv < 0)
    perror("send");

  return rv;
}

/**
 * Send a 404 response
 */
void resp_404(int fd, char *body)
{
  send_response(fd, "HTTP/1.1 404 NOT FOUND", "text/html", body);
}

/**
 * Send a / endpoint response
 */
void get_root(int fd)
{
  send_response(fd, "HTTP/1.1 200 OK", "text/html", "<h1>A Simple Web Server in C</h1>");
}

/**
 * Send a /d20 endpoint response
 */
void get_d20(int fd)
{
  srand(time(NULL));
  char random[sizeof("20")];
  sprintf(random, "%d", rand() % 20 + 1);
  send_response(fd, "HTTP/1.1 200 OK", "text/plain", random);
}

/**
 * Send a /date endpoint response
 */
void get_date(int fd)
{
  time_t t = time(NULL);
  struct tm *tm = gmtime(&t);
  int len = (sizeof(char) * 24) + 1;
  char date[len];

  strftime(date, len, "%c", tm);
  send_response(fd, "HTTP/1.1 200 OK", "text/plain", date);
}

/**
 * Post /save endpoint data
 */
void post_save(int fd, char *body)
{
  if (*body)
  {
    int pfd = open(POST_FILENAME, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int bytes_written = write(pfd, body, strlen(body));
    char *status = bytes_written < 0 ? "{\"status\" : \"failed\"}" : "{\"status\" : \"ok\"}";

    close(pfd);
    send_response(fd, "HTTP/1.1 200 OK", "application/json", status);
  } else {
    resp_404(fd, "<h1>Cannot POST /save</h1>");
  }
}

/**
 * Search for the start of the HTTP body.
 */
char *find_start_of_body(char *header)
{
  char *nn = "\n\n";
  char *rr = "\r\r";
  char *rn = "\r\n\r\n";
  char *pos;

  if ((pos = strstr(header, nn)) != NULL)
    return pos + strlen(nn);
  if ((pos = strstr(header, rr)) != NULL)
    return pos + strlen(rr);
  if ((pos = strstr(header, rn)) != NULL)
    return pos + strlen(rn);

  return NULL;
}

/**
 * Handle HTTP request and send response
 */
void handle_http_request(int fd)
{
  char request[REQ_BUFFER_SIZE];
  char request_type[8];
  char request_path[1024];
  char request_protocol[128];
  char resp_404_body[1066];
  char *request_body;

  int bytes_recvd = recv(fd, request, REQ_BUFFER_SIZE - 1, 0);

  if (bytes_recvd < 0) {
    perror("recv");
    return;
  }

  request[bytes_recvd] = '\0';
  sscanf(request, "%s %s %s", request_type, request_path, request_protocol);
  
  if (strcmp(request_type, "GET") == 0)
  {
    if (strcmp(request_path, "/") == 0)
      return get_root(fd);
    if (strcmp(request_path, "/d20") == 0)
      return get_d20(fd);
    if (strcmp(request_path, "/date") == 0)
      return get_date(fd);

    sprintf(resp_404_body, "<h1>Cannot GET %s</h1>", request_path);
    resp_404(fd, resp_404_body);
  }
  else if (strcmp(request_type, "POST") == 0)
  {
    if (strcmp(request_path, "/save") == 0)
    {
      request_body = find_start_of_body(request);
      return post_save(fd, request_body);
    }

    sprintf(resp_404_body, "<h1>Cannot POST %s</h1>", request_path);
    resp_404(fd, resp_404_body);
  }

  sprintf(resp_404_body, "<h1>Cannot %s %s</h1>", request_type, request_path);
  resp_404(fd, resp_404_body);
}

/**
 * Main
 */
int main(void)
{
  int newfd;                          // will hold socket descriptor for each new connection
  struct sockaddr_storage their_addr; // connector's address information
  char s[INET6_ADDRSTRLEN];

  start_reaper();

  int listenfd = get_listener_socket(PORT); // will listen for new connections
  if (listenfd < 0) {
    fprintf(stderr, "webserver: fatal error getting listening socket\n");
    exit(1);
  }

  printf("webserver: parent %d waiting for connections...\n", (int)getpid());

  // This is the main loop that accepts incoming connections and
  // fork()s a handler process to take care of it. The main parent
  // process then goes back to waiting for new connections.
  while(1) {
    socklen_t sin_size = sizeof their_addr;
    newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);

    if (newfd == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(
      their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s,
      sizeof s
    );

    printf("server: got connection from %s\n", s);

    if (fork() == 0)
    {
      printf("child %d processing request\n", (int)getpid());
      handle_http_request(newfd);
      exit(0);
    }

    close(newfd);
  }

  return 0;
}

