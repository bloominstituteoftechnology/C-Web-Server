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
    // Build HTTP response and store it in response
   
   // The total length of the header **and** body should be stored in the `response_length` variable 
   // so that the `send()` call knows how many bytes to send out over the wire.

   // Hint: `sprintf()` for creating the HTTP response. `strlen()` for computing content length. 
   // `sprintf()` also returns the total number of bytes in the result string, which might be helpful. For getting the current time for the Date field of the response, you'll want to look at the `time()` and `localtime()` functions, both of which are already included in the `time.h` header file. 

    // The HTTP `Content-Length` header only includes the length of the body, 
    // not the header. But the `response_length` variable used by `send()` 
    // is the total length of both header and body.

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    const int max_response_size = 65536;
    char response[max_response_size];  

    // http://www.java2s.com/Code/C/Development/ConverttimetvaluetotmstructureinlocaltimeHowtouselocaltime.htm
    time_t rawtime;
    struct tm *tminfo;
    time(&rawtime);
    tminfo = localtime ( &rawtime );

    int response_length = sprintf(response,
      "Header: %s\n"
      "Date: %s"
      "Content-Type: %s\n"
      "Content-Length: %d\n"
      "Body:\n%s\n",

      header,
      asctime(tminfo),
      content_length,
      content_type,
      body);

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
    
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    srand(getpid() + time(NULL)); // seed random numbers
    char response_body[8];
    sprintf(response_body, "%d\n", (rand()% 20) + 1);

    // Use send_response() to send it back as text/plain data

    send_response(fd, "HTTP/1.1 200 OK", "text/plain", response_body, sizeof(response_body));
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

// When a file is requested, first check to see if the path to the file is in
// the cache (use the file path as the key).

// If it's there, serve it back.

// If it's not there:

// Load the file from disk (see `file.c`)
// Store it in the cache
// Serve it

/**
 * Read and return a file from disk or cache
 */
void get_file(int fd, struct cache *cache, char *request_path)
{
//    Any other URL should map to the `serverroot` directory and files that lie
//    within. For example:

//    `http://localhost:3490/index.html` serves file `./serverroot/index.html`.

//    `http://localhost:3490/foo/bar/baz.html` serves file
//    `./serverroot/foo/bar/baz.html`.

//    You might make use of the functionality in `file.c` to make this happen.

//    You also need to set the `Content-Type` header depending on what data is in
//    the file. `mime.c` has useful functionality for this.

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    char filepath[4096];
    struct file_data *filedata; 
    char *mime_type;

    // Fetch the 404.html file
    snprintf(filepath, sizeof filepath, "%s%s", SERVER_ROOT, request_path);
    

    
    filedata = file_load(filepath);

    if (filedata == NULL) {
        // TODO: make this non-fatal
        fprintf(stderr, "cannot find system 404 file\n");
        exit(3);
    }

    mime_type = mime_type_get(filepath);

    send_response(fd, "HTTP/1.1 OK", mime_type, filedata->data, filedata->size);

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
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];
    char request_type[8]; // ex. GET or POST
    char request_path[1024]; // ex. /d20 or /date
    char request_protocol[16];

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0) {
        perror("recv");
        return;
    }

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    sscanf(request, "%s %s %s", request_type, request_path, request_protocol);

    printf("Got request: %s %s %s\n:", request_type, request_path, request_protocol);

    if (strcmp(request_type, " GET"))
    {
        if (strcmp(request_path, "/d20") == 0)
        {
            printf("d20 has been reached\n");
            get_d20(fd);
        }
        else if (strcmp(request_path, "/") == 0)
        {
            printf("Root has been reached\n");
            get_file(fd, cache, "index.html");
        }
        else
        {
            printf("404 has been reached\n");
            resp_404(fd);
        }
    }

    // Read the three components of the first request line

    // sscanf(request, "%s %s %s",  request_type, request_path, request_protocol); // break down the first line of the request buffer into its pieces

    // // printf("Got request: %s %s %s", request_type, request_path, request_protocol);

    // // If GET, handle the get endpoints

    // if (strcmp(request_type, "GET") == 0) {
    //     if(strcmp(request_path, "/d20") == 0) {
    //         get_d20(fd); // pass in file descriptor
    //     } else {
    //         // attempt to retrieve the requested file
    //         get_file(fd, cache, request_path);
    //     }
    // }

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

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}

