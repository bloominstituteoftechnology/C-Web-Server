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
/* EXAMPLE RESPONSE
HTTP/1.1 200 OK
Date: Wed Dec 20 13:05:11 PST 2017
Connection: close
Content-Length: 41749
Content-Type: text/html

<!DOCTYPE html><html><head><title>Lambda School ...

*** The end of the header on both the request and response is marked by a blank line (i.e. two newlines in a row).
*/
void get_time(char *return_time)
{
    time_t current_time;
    struct tm *local_time;
    char *created_time; 

    current_time = time(NULL);
    local_time = localtime( &current_time );
    // printf("%s\n", asctime (local_time));
    created_time = asctime (local_time);
    // printf("created_time: %s\n", created_time);
    sprintf(return_time,"%s", created_time);
    // printf("created_time after sprintf: %s\n", created_time);
    *return_time = *created_time;
    // NO NEED TO RETURN
}
int send_response(int fd, char *header, char *content_type, void *body, int content_length)
{
    /*
        //  (void)header;
        //  (void)content_type;
        //  (void)body;
        //  (void)content_length;
        //  (void)fd;
        //  (void)response;
        //  (void)max_response_size;
    */
    const int max_response_size = 65536;
    char response[max_response_size];

    // Build HTTP response and store it in response

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    char return_time[80];
    get_time(return_time);
    // printf("return_time from send():%s\n", return_time);
    // printf("sizeof:%ld\n", sizeof(return_time));
    int res_len = sprintf(response, "HTTP/1.1 %s\nDate:%sConnection: close\nContent-Length: %i\nContent-Type: %s\n\n%s\n\n", header, return_time, content_length, content_type, body);
    // printf("dest:%s\n", body);
    printf("  <-- <start of res>\n%s    <end of res>\n", response);

    int response_length = res_len;
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
    // (void)fd;
    // Generate a random number between 1 and 20 inclusive
    
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    int random_num;
    srand(time(NULL));
    random_num = rand() % 20 +1;
    // printf("%i\n", random_num);

    // (void)random_num;

    // Use send_response() to send it back as text/plain data

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    // send_response(int fd, char *header, char *content_type, void *body, int content_length)
    //send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);
// ^- REF ONLY
    char body[2];    
    int len_body = sprintf(body, "%i", random_num);
    // printf("src:%i\n", random_num);
    // printf("1:%i\n", ret_val);
     // BELOW, SAME VALUE AS RETURNED FROM SPRINTF
     // int ret_val_two = strlen(body);
    // printf("2:%i\n", ret_val_two);
    // printf("3:%i\n", len_body);

    send_response(fd, "201 CREATED", "text/plain", body, len_body);
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

    // send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);
    send_response(fd, "404 NOT FOUND", mime_type, filedata->data, filedata->size);

    file_free(filedata);
}

/**
 * Read and return a file from disk or cache
 */
void get_file(int fd, struct cache *cache, char *request_path)
{
    //  (void)fd;
     (void)cache;
    //  (void)request_path;
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    char filepath[4096];
    // filepath = request_path;
    struct file_data *filedata; 
    char *mime_type;

    
    printf("request_path: %s\n", request_path);

    snprintf(filepath, sizeof filepath, "%s%s", SERVER_ROOT, request_path);
    printf("filepath: %s\n", filepath);
    filedata = file_load(filepath);

    if (filedata == NULL) {
        // TODO: make this non-fatal
        fprintf(stderr, "ERROR: cannot find system %s file\n", filepath);
        resp_404(fd);
        exit(3);
    }
    mime_type = mime_type_get(filepath);
    printf("mime_type: %s\n", mime_type);
    // printf("filesize: %s\n", filedata->size);
    // printf("filedata: %s\n", filedata->data);
    send_response(fd, "200 OK", mime_type, filedata->data, filedata->size);
    file_free(filedata);
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
     (void)header;
    ///////////////////
    // IMPLEMENT ME! // (Stretch)
    ///////////////////
     return(0);
}

/**
 * Handle HTTP request and send response
 */
void handle_http_request(int fd, struct cache *cache)
{
     (void)cache;
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
     // printf("--> incoming request:\n%s<end of req>\n",request);
    // RETURNS:
        // GET /test HTTP/1.1
        // Host: localhost:3490
        // User-Agent: curl/7.61.1
        // Accept: */*

    // Read the three components of the first request line
    char req_type[8];
    char req_endpoint[4096];
    // SSCANF BREAKS DOWN THE REQ INTO VARs
    sscanf(request, "%s %s", req_type, req_endpoint);
    // printf("req_type: %s\nreq_endpoint: %s\n", req_type, req_endpoint);
    printf("  --> incoming request: %s %s\n", req_type, req_endpoint);

    if (strlen(req_endpoint) > 1) {
        // MEMMOVE, REMOVES THE LEADING '/'
        memmove(req_endpoint, req_endpoint+1, strlen(req_endpoint));
        // printf("NEW req_endpoint: %s\n", req_endpoint);
    }
    // printf("NEW req_endpoint: %s\n", req_endpoint);

    // If GET, handle the get endpoints

    if (strcmp(req_type, "GET") == 0) {
        // printf("found GET in req\n");
        if (strcmp(req_endpoint, "/") == 0 || strcmp(req_endpoint, "index.html") == 0 || strcmp(req_endpoint, "home") == 0) {
            printf("found HOME\n");

            get_file(fd, cache, "/index.html");

            exit(0);
        }
    //    Check if it's /d20 and handle that special case
        else if (strcmp(req_endpoint, "d20") == 0) {
            // printf("found d20 in endpoint\n");
            get_d20(fd);
        }
    //    Otherwise serve the requested file by calling get_file()

    }

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

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}

