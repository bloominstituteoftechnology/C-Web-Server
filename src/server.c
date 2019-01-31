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
#include <stdint.h>
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
int send_response(int fd, char *header, char *content_type, void *body, int content_length)
{
    const int max_response_size = 265536;
    char response[max_response_size];
    time_t date_time = time(NULL);

    // Build HTTP response and store it in response
    int response_length = sprintf(response,
        "%s\nDate: %sConnection: close\nContent-Length: %d\nContent-Type: %s\n\n",
        header,
        asctime(gmtime(&date_time)),
        content_length,
        content_type
    );

    memcpy(response + response_length, body, content_length);

    // Send it all!
    int rv = send(fd, response, response_length + content_length, 0);

    if (rv < 0) {
        perror("send");
    }

    return rv;
}


/**
 * Send a /d20 endpoint response
 */
void get_d20(int fd)
{
    srand(time(0));
    int lower = 1;
    int upper = 20;
    char result[10];

    // Generate a random number between 1 and 20 inclusive
    int d20 = (rand() % (upper - lower + 1)) + lower;

    sprintf(result,"%d\n",d20);

    // Use send_response() to send it back as text/plain data
    send_response(fd, "HTTP/1.1 200 OK", "text/plain", result, strlen(result));
}

/**
 * Send a 404 response
 */
void resp_404(int fd)
{
    char filepath[4096];
    struct file_data *filedata; 
    char *mime_type;

    // Fetch the 404.html file
    snprintf(filepath, sizeof filepath, "%s/404.html", SERVER_FILES);
    filedata = file_load(filepath);

    if (filedata == NULL) {
        // TODO: make this non-fatal
        fprintf(stderr, "cannot find system 404 file\n");
        exit(3);
    }

    mime_type = mime_type_get(filepath);

    send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);

    file_free(filedata);
}

/**
 * Read and return a file from disk or cache
 */
void get_file(int fd, struct cache *cache, char *request_path)
{
    char *mime_type;
    char path_buffer[1024];
    struct cache_entry *ce;
    struct file_data *filedata;

    ce = cache_get(cache, request_path);

    if (ce)
    {
        send_response(fd, "HTTP/1.1 200 OK", ce->content_type, ce->content, ce->content_length);
        return;
    }

    snprintf(path_buffer, sizeof(path_buffer), "%s%s", SERVER_ROOT, request_path);
    filedata = file_load(path_buffer);

    if (filedata)
    {
        mime_type = mime_type_get(request_path);
        cache_put(cache, request_path, mime_type, filedata->data, filedata->size);
        send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);
        file_free(filedata);
        return;
    }

    if (!strcmp(request_path, "/")) // if the strings match
    {
        strcat(request_path, "index.html");
        snprintf(path_buffer, sizeof(path_buffer), "%s%s", SERVER_ROOT, request_path);
        filedata = file_load(path_buffer);

        if (filedata)
        {
            mime_type = mime_type_get(request_path);
            cache_put(cache, request_path, mime_type, filedata->data, filedata->size);
            send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);
            file_free(filedata);
            return;
        }
    }

    resp_404(fd);
    return;
}

/**
 * Search for the end of the HTTP header
 * 
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 */
char *find_start_of_body(char *header)
{
    char *body = strstr(header, "\n\r");
    body += 3;
    return body;
}

void post_save(char *req_path, char *body, long content_length)
{
    int fd;
    char path_buffer[1024];

    snprintf(path_buffer, sizeof(path_buffer), "%s%s", SERVER_ROOT, req_path);

    fd = open(path_buffer, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd < 0) {
        fprintf(stderr, "fd < 0\n");
        exit(-1);
    }

    flock(fd, LOCK_EX);

    write(fd, body, content_length);

    flock(fd, LOCK_UN);

    close(fd);
}

/**
 * Handle HTTP request and send response
 */
void handle_http_request(int fd, struct cache *cache)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];
    char req_method[8], req_path[1024], req_protocol[128];
    char *body;
    long content_length;

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0) {
        perror("recv");
        return;
    }

    // Read the three components of the first request line
    sscanf(request, "%s %s %s", req_method, req_path, req_protocol);

    // If GET, handle the get endpoints
    if (strcmp(req_method, "GET") == 0)
    {
        // Check if it's /d20 and handle that special case
        if (strcmp(req_path, "/d20") == 0)
        {
            get_d20(fd);
            return;
        }
        // Otherwise serve the requested file by calling get_file()
        get_file(fd, cache, req_path);
        return;
    }

    // (Stretch) If POST, handle the post request
    if (strcmp(req_method, "POST") == 0)
    {
        body = find_start_of_body(request);
        content_length = strlen(body);
        post_save(req_path, body, content_length);
        send_response(fd, "HTTP/1.1 201 Created", "application/json", "{\"status\":\"ok\"}", 15);
        return;
    }

    resp_404(fd);
    return;
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

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}

