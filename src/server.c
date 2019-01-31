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
int send_response(int fd, char *header, char *content_type, void *body, int content_length)
{
    const int max_response_size = 2000000;
    char response[max_response_size];

    // SAMPLE: 
    // HTTP/1.1 200 OK
    // Date: Wed Dec 20 13:05:11 PST 2017
    // Connection: close
    // Content-Length: 41749
    // Content-Type: text/html

    // Build HTTP response and store it in response
    // HINT: use sprintf() for creating response.
    // HINT: use strlen() for computing content length.
    // HINT: sprintf() returns number of bytes in result string.
    // HINT: time() and localtime().


    // Reference: http://www.cplusplus.com/reference/ctime/localtime/
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    int response_length = sprintf(response, "%s\nDate: %sConnection: close\nContent-Length: %i\nContent-Type: %s\n\n", header, asctime(timeinfo), content_length, content_type);
    // printf("%s\n", response);
    // printf("%i\n", response_length);
    // printf("%i\n", content_length);
    
    memcpy(response + response_length, body, content_length);
    response_length += content_length;

    // printf("%i\n", response_length);
    // printf("%i\n", content_length);

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // Send it all!
    int rv = send(fd, response, response_length, 0);

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
    // Generate a random number between 1 and 20 inclusive
    srand(time(NULL)); // initialize the random seed using the current time as seed

    char response[4]; // initialize the string that will store our int d20 value

    int d20 = (rand() % 20) + 1; // generate an int between 1-20 inclusive by using +1

    sprintf(response, "%i\n", d20); // translate the d20 value into a string

    printf("%i\n", d20); // console log the number for testing

    // Use send_response() to send it back as text/plain data
    send_response(fd, "HTTP/1.1 200 OK", "text/plain", response, strlen(response)); // send the number string and its length in plaintext to send_response
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

    // USAGE: cache_get(cache, path)
    struct cache_entry *cache_item = cache_get(cache, request_path);

    if(cache_item != NULL){
        printf("Loading from cache...\n");
        send_response(fd, "HTTP/1.1 200 OK", cache_item->content_type, cache_item->content, cache_item->content_length);
    } else {
    struct file_data *filedata = NULL;
    char *mime_type;
    char full_path[256];

    sprintf(full_path, "./serverroot%s", request_path);
    printf("%s\n", full_path);

    filedata = file_load(full_path);
    mime_type = mime_type_get(full_path);

    // verify filedata returns something
        if(filedata != NULL){
            // if data, add to cache, and call send_response
            cache_put(cache, request_path, mime_type, filedata->data, filedata->size);
            printf("Loading from server...\n");

            send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);

            // free the filedata, since we now have it in the cache
            file_free(filedata);
        } else {
            resp_404(fd);
        }

    }


    
    
    
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
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

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0) {
        perror("recv");
        return;
    }


    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // HINT: strcmp() will match request methods and path. Returns 0 if the same.
    // Use an if-else block with strcmp().
    // HINT: Use sscanf() to read the contents.

    // Read the three components of the first request line
    // printf("Request: %s\n", request);

    char method[8], path[32], protocol[16]; // initialize strings for our method, path, and protocol values
    sscanf(request, "%s %s %s", method, path, protocol); // store the strings from the request header
    // printf("%s, %s, %s\n", method, path, protocol);

    if(strcmp(method, "GET") == 0 && strcmp(path, "/d20") == 0){
        get_d20(fd); // call d20 if /d20 path is found with GET method
        printf("get_d20() was called.\n");
    } else if(strcmp(method, "GET") == 0 && strcmp(path, "/") == 0){
        get_file(fd, cache, "/index.html");
    } else if(strcmp(method, "GET") == 0){
        printf("get_file() was called.\n");
        get_file(fd, cache, path); // search for the path passed in the request
    } else {
        resp_404(fd);
    }

    // If GET, handle the get endpoints

    //    Check if it's /d20 and handle that special case
    //    Otherwise serve the requested file by calling get_file()


    // (Stretch) If POST, handle the post request
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

        // test 404 page
        // resp_404(newfd);

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}

