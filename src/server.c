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

#define PORT "3490" // the port users will be connecting to

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
    const int max_response_size = 262144;
    char response[max_response_size];

    // Build HTTP response and store it in response

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // time_t is an arithmetic type suitable to represent time
    time_t secs;
    // struct tm is a structure type for holding components of calendar time
    struct tm *time_info;

    time(&secs);
    time_info = localtime(&secs);

    char *body_str = body;

    // // Marker for time
    // time_t time_res_sent = time(NULL);

    // Response length
    int response_length = sprintf(
        response, "%s\nDate: %sConnection: close\nContent-Length: %d\nContent-Type: %s\n\n",
        header, asctime(time_info),

        // asctime() returns a string containing the date and time information
        // // Get the time
        // header, asctime(localtime(&time_res_sent)),
        content_length,
        content_type);

    // printf("Date: %s\n", response_length + content_length);
    printf("Connection: close\n", response_length + content_length);
    printf("Content-Length: %d\n", response_length + content_length);

    // memcpy(void *to, const void *from, size_t n)
    // Copies n from a memory area pointed to by 'from' to a memory area pointed to by 'to'
    memcpy(response + response_length, body, content_length);

    // New response_length
    response_length += content_length;

    // Send it all!
    int rv = send(fd, response, response_length, 0);

    if (rv < 0)
    {
        perror("send");
    }

    return rv;
}

/**
 * Send a /d20 endpoint response
 */
void get_d20(int fd)
{
    // Generate a random number between 1 and 20 inclusive

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // Use send_response() to send it back as text/plain data

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
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

    if (filedata == NULL)
    {
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
// request_path should map to file name in serverroot directory
// use methods in file.c to load/read the corresponding file
// use methods in mime.c to set the content-type header based on the type of data in the file
void get_file(int fd, struct cache *cache, char *request_path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // Pointer to a file_data struct
    struct file_data *filedata = NULL;
    // Pointer to a mime_type aka content_type
    char *mime_type;
    // Declare an array to store the file path
    char file_path[4096];

    // Use request_path for the cache; use full file_path (./serverroot + req_path) for loading files not in the cache
    // Get the specified entry from the cache
    struct cache_entry *entry = cache_get(cache, request_path);

    // If cache has no entry with the given request_path
    if (entry == NULL)
    {
        // Build a full file path into the ./serverroot directory
        sprintf(file_path, "./serverroot%s", request_path);
        // Returns filedata->data and filedata->size
        filedata = file_load(file_path);

        // If user inputs '/' as the path, and serve index.html file
        if (filedata == NULL)
        {
            sprintf(file_path, "./serverroot%s/index.html", request_path);
            filedata = file_load(file_path);
            if (filedata == NULL)
            {
                resp_404(fd);
                return;
            }
        }

        // else filedata != NULL
        // Checks the file extension and returns `content-type` string
        mime_type = mime_type_get(file_path);

        // Put an entry in cache for the given file
        cache_put(cache, request_path, mime_type, filedata->data, filedata->size);

        send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);
        file_free(filedata);
        return;
    }
    // else entry != NULL:

    send_response(fd, "HTTP/1.1 200 OK", entry->content_type, entry->content, entry->content_length);
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
    ///////////////////
    // IMPLEMENT ME! // (Stretch)
    ///////////////////
}

/**
 * Handle HTTP request and send response
 */
void handle_http_request(int fd, struct cache *cache)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];

    // Buffers for the request:
    char req_method[200]; // HOST, HTTP/1.1
    char req_type[8];     // GET, POST, etc.
    char req_path[2048];  // URL path info, for /d20

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0)
    {
        perror("recv");
        return;
    }

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // Read the first two components of the first line of the request
    sscanf(request, "%s %s %s", req_type, req_path, req_method);

    printf("\nHTTP Request: \nType:%s \nPath: %s \nMethod:%s\n", req_type, req_path, req_method);

    // strcmp() compares the two strings character by character
    // starting from the first character until the characters in both strings are equal
    // or a NULL character is encountered

    // If GET, handle the get endpoints
    if (strcmp(req_type, "GET") == 0)
    {
        // Check if it's /d20 and handle that special case
        if (strcmp(req_path, "/d20") == 0)
        {
            get_d20(fd);
        }
        else
        {
            // If path is not /d20, then get file at the path specified
            get_file(fd, cache, req_path);
        }
    }
    else
    {
        // If not GET, respond with 404
        resp_404(fd);
    }

    //    Otherwise serve the requested file by calling get_file()

    // (Stretch) If POST, handle the post request
}

/**
 * Main
 */
int main(void)
{
    int newfd;                          // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];

    struct cache *cache = cache_create(10, 0);

    // Get a listening socket
    int listenfd = get_listener_socket(PORT);

    if (listenfd < 0)
    {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }

    printf("webserver: waiting for connections on port %s...\n", PORT);

    // This is the main loop that accepts incoming connections and
    // responds to the request. The main parent process
    // then goes back to waiting for new connections.

    while (1)
    {
        socklen_t sin_size = sizeof their_addr;

        // Parent process will block on the accept() call until someone
        // makes a new connection:
        newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1)
        {
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

        // // testing send_response
        // resp_404(newfd);

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}
