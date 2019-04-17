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

// int get_current_time() {
//         // gets current time as a string
//     time_t rawtime;
//     struct tm * timeinfo;
//     time (&rawtime);
//     timeinfo = localtime (&rawtime);
//     return timeinfo;
// }

void mem_copy(void *dest, const void *src, int n)
{
    // type cast to char
    char * chardest = (char*) dest;
    char * charsrc = (char*) src;
    // get num chars from bytes
    int length = n / sizeof(char);
    // copy values
    for (int i = 0; i < length; i++) {
        chardest[i] = charsrc[i];
    }
    chardest[length+1] = '\0';
}

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

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // Build HTTP response and store it in response

    // gets current time as a string
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    char * current_date = asctime(timeinfo);

    int header_length = sprintf(&response, "%s\n"
            "Date: %s"
            "Connection: close\n"
            "Content-Length: %d\n"
            "Content-Type: %s\n"
            "\n", header, current_date, content_length, content_type);

    // char *file_blob = malloc(content_length);
    printf("copying memory %d - %d\n", header_length, content_length);

    memcpy(response + header_length , body, content_length);

    // response[header_length + content_length+1] = '\0';

    printf("response\n------------\n%s\n-----------\n", response);

    // Send it all!
    int rv = send(fd, response, header_length + content_length, 0);

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

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // time_t rawtime;
    srand(time(NULL));
    int d20_val = rand();
    
    char output[100]; // one hundred just to be safe
    sprintf(output, "%d\0", d20_val);

    int output_len = strlen(output);//(int)((ceil(log10(d20_val))+1)*sizeof(char));
    printf("d20 roll: %f\n", d20_val);

    // Use send_response() to send it back as text/plain data
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    send_response(fd, "http/1.1 200 /d20", "text/plain", output, output_len);
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
    // printf("\"%s\"", filedata->data);
    send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);

    file_free(filedata);
}

/**
 * Read and return a file from disk or cache
 */
void get_file(int fd, struct cache *cache, char *request_path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    struct file_data *filedata;
    char *mime_type;
    char filepath[4096];


    // snprintf(request_path, sizeof request_path, "%s/index.html", SERVER_ROOT);
    snprintf(filepath, sizeof filepath, "%s%s", SERVER_ROOT, request_path);
    printf("getting file: %s\n", filepath);
    filedata = file_load(filepath);

    if (filedata == NULL) {
        fprintf(stderr, "cannot find %s\n", filepath);
        printf("path does not exist \n");
        resp_404(fd);
        // exit(3);
        return;
    }

    mime_type = mime_type_get(filepath);

    char header[64];
    sprintf(header, "HTTP/1.1 200 %s", request_path);
    printf("header: %s\n", header);

    send_response(fd, header, mime_type, filedata->data, filedata->size);
    file_free(filedata);
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
    const int request_buffer_size = 262144; // 256 kb
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

    // char *request_parts = strtok(request, " ");
    // printf("request verb: %s request endpoint: %s", request_parts[0],request_parts[1]);

    char request_type[10], path[50];
    sscanf(request, "%s %s", request_type, path);
    printf("request: %s %s\n", request_type, path);
    
    // parse header

    // Read the first two components of the first line of the request 
    // printf("request: %s\n", request);
 
    // If GET, handle the get endpoints
    // char *is_get = strstr(request, "GET");
    // char *route = strchr(request, '/');

    // printf("request endpoint: \"%s\" \"%s\"\n", is_get, route);

    char filepath[4096];
    struct file_data *filedata;
    char *mime_type;


    if (strcmp(request_type, "GET") == 0 && strcmp(path, "/index.html") == 0) 
    {
        printf("is get\n");

        snprintf(filepath, sizeof filepath, "%s/index.html", SERVER_ROOT);
        filedata = file_load(filepath);

        if (filedata == NULL) {
            fprintf(stderr, "cannot find index.html\n");
            exit(3);
        }

        mime_type = mime_type_get(filepath);

        send_response(fd, "HTTP/1.1 200 /index.html", mime_type, filedata->data, filedata->size);
        file_free(filedata);
    }
    else if (strcmp(request_type, "GET") == 0 && strcmp(path, "/d20") == 0) 
    {

        get_d20(fd);
    }
    else 
    {

        // snprintf(filepath, sizeof filepath, "%s%s", SERVER_ROOT, path);
        // printf("%s", filepath);

        // check if file exists?
        get_file(fd, cache, path);

    }
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
    // responds to the request. The main parent process
    // then goes back to waiting for new connections.
    
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

