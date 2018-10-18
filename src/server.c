/**
 * webserver.c -- A webserver written in C
 * 
 * Test with curl (if you don't have it, install it):
 * 
 *     curl -D - http://localhost:3490/
 *     curl -D - http://localhost:3490/d20
 *     curl -D - http://localhost:3490/index.html
 * 
 * You can also test the above URLs in your browser! They should work!
 * 
 * Posting Data:
 *     curl - d -X POST -H 'Content-Type: text/plain' -d 'Hello, sample data!' http://localhost3490/save
 * 
 * (Posting data is harder to test from a browser.)
 **/
// so this is a server
// we are getting information from a client, parsing that information
// and then sending a response back to the client based on that request


// here is a bunch of stuff that we imported
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

#define PORT "3490" // the port users connect to
// these are constant variables
#define SERVER_FILES "./serverfiles"
#define SERVER_ROOT "./serverroot"

/**
 * Send and HTTP response
 * 
 * fd:              The file descriptor of the socket to send the response through.
 * header:          "HTTP/1.1 404 NOT FOUND" or HTTP/1.1 200 OK", etc.
 * content_type:    "text/plain", etc.
 * body:            The data to send.
 * content_length:  The length of the data in the body.
 * 
 * Return the value from the send() function.
 **/
// So here we're going to send a response
// so 5 parameters are being passed into this function:
// fd, a pointer to header, a pointer to content_type, a pointer to body, and content_length
int send_response(int fd, char *header, char *content_type, void *body, int content_length)
{
    // declare an integer called max_response_size
    const int max_response_size = 65536;
    char response[max_response_size]; //char variable with memory allocated

    // Get current time for the HTTP header
    // now I don't get this syntax time_t t1 = time(NULL)
    time_t t1 = time(NULL);
    // I think this is saying, create a struct called tm that is really a pointer called *ltime that
    // points to localtime(&t1)??? wtf???
    struct tm *ltime = localtime(&t1);
    
    // this is also pretty strange...response_length is just an integer...
    // but sprintf puts stuff into the response char array...
    // so how does response_length become an integer equal to the length of the response???
    int response_length = sprintf(response,
        // so we have five format thingys, %s, %d, etc
        "%s\n"
        "Date: %s" // asctime adds own newline
        "Connection: close\n"
        "Conent-Length: %d\n"
        "Content-Type: %s\n"
        "\n"
        "%s\n",
        // and down here we have five variables that we are stuffing into those format thingys...
        // makes sense.
        header,
        asctime(ltime),
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
 **/
// this one actually makes sense to me...
void get_d20(int fd)
{
    // except for this part. What the hell is this line doing?
    srand(time(NULL) + getpid());
    // create a char array called response_body and allocate 8 bytes of memory to it...?
    char response_body[8];
    // now shove stuff into that response_body with sprintf
    // in this case it's a random number between 1-20
    sprintf(response_body, "%d\n", (rand()%20)+1);
    // and now we send the response
    // I see it takes 5 parameters, the file descriptor, an HTTP message, the response body,
    // and the length of the response body
    send_response(fd, "HTTP/1.1 200 OK", "text/plain", response_body, strlen(response_body));
}

/**
 * Send a 404 response
 **/
void resp_404(int fd)
{   
    // create a char array called filepath and allocated 4096 bytes of space to it
    char filepath[4096];
    // create a struct called file_data that is a pointer to *filedata...?
    struct file_data *filedata;
    // create a char pointer called *mime_type???
    char *mime_type;
    // so I'm reading up about snprintf and it says it is basically a function that redirects
    // the output of printf to a buffer...so I think that in English that means its taking stuff
    // and putting it into our char array called filepath
    snprintf(filepath, sizeof filepath, "%s/404.html", SERVER_FILES);
    // so I looked at this function in file.c, and I believe it's just opening a file
    // and giving us access to the contents of that file, hence the name filedata
    filedata = file_load(filepath);
    // now we ask a question...is the file empty? or does the file not exist???
    if (filedata == NULL) {
        // TODO: make this non-fatal
        // if so, then print an error and exit
        fprintf(stderr, "cannot find system 404 file\n");
        exit(3);
    }
    // MIME = Multipurpose Internet Mail Extensions???
    // not really sure what this is doing
    // I think it's assigning a MIME type to the filepath...not really sure 
    mime_type = mime_type_get(filepath);
    // now we send the response
    send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);
    // and then we free the memory associated with filedata
    file_free(filedata);
}

/**
 * Read and return a cache entry or file
 **/
// so the name pretty much explains what's going on here...
// see if the file exists in cache, if not then open the file???
int get_file_or_cache(int fd, struct cache *cache, char *filepath)
{   // create a struct called file_data that is a pointer to *filedata???
    struct file_data *filedata;
    // create a struct called cache_entry that is a pointer to cacheent???
    struct cache_entry *cacheent;
    // create a pointer called *mime_type that is a char pointer
    char *mime_type;

    // Try to find the file
    // so we check the cache and see if the data is already there...?
    cacheent = cache_get(cache, filepath);
    
    if (cacheent != NULL) {
        // Found it in the cache

        // TODO: remove and ignore the cache entry if it's too old

        send_response(fd, "HTTP/1.1 200 OK", cacheent->content_type, cacheent->content, cacheent->content_length);

    } else {
        // otherwise, we just load the file
        filedata = file_load(filepath);

        if (filedata == NULL) {
            return -1; // failure
        }
        // and again, we have to get the mime type...whatever that means...
        mime_type = mime_type_get(filepath);
        // and now we send the response back to the user
        send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);

        // Save it for next time
        // so here we're putting it into the cache for faster access in the future
        // but what is the cache? is it the linked list? the hash table? or both? 
        cache_put(cache, filepath, mime_type, filedata->data, filedata->size);
        // and then we free it
        file_free(filedata);
    }

    return 0; // success
}

/**
 * Read and return a file
 **/
void get_file(int fd, struct cache *cache, char *request_path)
{
    // create a char array called filepath with 4096 bytes of memory
    char filepath[4096];
    // create an integer called status
    int status;

    // Try to find the file
    snprintf(filepath, sizeof filepath, "%s%s/index.html", SERVER_ROOT, request_path);
    status = get_file_or_cache(fd, cache, filepath);

    if (status == -1) {
        snprintf(filepath, sizeof filepath, "%s%s/index.html", SERVER_ROOT, request_path);
        status = get_file_or_cache(fd, cache, filepath);

        if (status == -1) {
            resp_404(fd);
            return;
        }
    }
}

/**
 * Post /save endpoint data
 **/
void post_save(int fd, char *body)
{
    char *status;

    // Open the file
    int file_fd = open("data.txt", O_CREAT|O_WRONLY, 0644);

    if (file_fd >= 0) {
        // Exclusive lock to keep processes from trying to write the file at the 
        // same time. This is only necessary if we've implemented a
        // multiprocessed version with fork().
        flock(file_fd, LOCK_EX);

        // Write body
        write(file_fd, body, strlen(body));

        // Unlock
        flock(file_fd, LOCK_UN);

        // Close
        close(file_fd);

        status = "ok";
    } else {
        status = "fail";
    }

    // Now send an HTTP response

    char response_body[128];

    sprintf(response_body, "{\"status\": \"%s\"}\n", status);

    send_response(fd, "HTTP/1.1 200 OK", "application/json", response_body, strlen(response_body));
}

/**
 * Search for the end of the HTTP header
 * 
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 **/
char *find_start_of_body(char *header)
{
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
 **/
void handle_http_request(int fd, struct cache *cache)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];
    char *p;
    char request_type[8]; // GET or POST
    char request_path[1024]; // /info etc.
    char request_protocol[128]; // HTTP/1.1

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0) {
        perror("recv");
        return;
    }

    // NUL terminate request string
    request[bytes_recvd] = '\0';

    // Look for two newlines marking the end of the header
    p = find_start_of_body(request);

    if (p == NULL) {
        printf("Could not find the end of header\n");
        exit(1);
    }

    // And here is the body
    char *body = p;

    
    // Now that we've assessed the request, we can take actions.


    // Read the three components of the first request line
    sscanf(request, "%s %s %s", request_type, request_path, request_protocol);


    printf("REQUEST: %s %s %s\n", request_type, request_path, request_protocol);

    if (strcmp(request_type, "GET") == 0) {
        if (strcmp(request_path, "/d20") == 0) {
            // Handle any programmatic endpoints here
            get_d20(fd);
        } else {
            // Otherwise try to get a file
            get_file(fd, cache, request_path);
        }
    }
    else if (strcmp(request_type, "POST") == 0) {
        // Endpoint "/Save"
        if (strcmp(request_path, "/save") == 0) {
            post_save(fd, body);
        } else {
            resp_404(fd);
        }
    }

    else {
        fprintf(stderr, "unknown request type \"%s\"\n", request_type);
        return;
    }
}




/**
 * Main
 **/

int main(void)
{
    int newfd; // listen on sock_fd, new connection on newfd
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

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // wtf
    return 0;
}