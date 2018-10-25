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
#include "server.h"
#include <pthread.h>

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
    const int max_response_size = 65536;
    char response[max_response_size];

    time_t rawtime = time(&rawtime);
    struct tm *info = localtime(&rawtime);

    int response_length = sprintf(response, "%s\r\nDate: %sConnection: close\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n", header, asctime(info), content_length, content_type);
    memcpy(response + response_length, body, content_length);
    int rv = send(fd, response, response_length + content_length, 0);

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
    srand(getpid() + time(NULL));
    int random_number = rand() % 20 + 1;
    char body[20];
    sprintf(body, "%d", random_number);

    send_response(fd, "HTTP/1.1 200 OK", "text/plain", body, strlen(body));
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
void get_file(int fd, struct cache *cache, char *request_path)
{
    char filepath[4096];
    struct file_data *filedata;
    char *mime_type;

    snprintf(filepath, sizeof filepath, "%s%s", SERVER_ROOT, request_path);

    struct cache_entry *entry = cache_get(cache, filepath);

    if (entry != NULL)
    {
        send_response(fd, "HTTP/1.1 OK", entry->content_type, entry->content, entry->content_length);
    }
    else
    {
        filedata = file_load(filepath);

        if (filedata == NULL)
        {
            snprintf(filepath, sizeof filepath, "%s%sindex.html", SERVER_ROOT, request_path);
            filedata = file_load(filepath);

            if (filedata == NULL)
            {

                fprintf(stderr, "Cannot find system file: %s\n", request_path);
                resp_404(fd);
                return;
            }
        }

        mime_type = mime_type_get(filepath);

        send_response(fd, "HTTP/1.1 OK", mime_type, filedata->data, filedata->size);
        cache_put(cache, filepath, mime_type, filedata->data, filedata->size);

        file_free(filedata);
    }
}

/**
 * Search for the end of the HTTP header
 * 
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 */
char *find_start_of_body(char *header)
{
    char *body = strstr(header, "\r\n\r\n"); // First two CRLF's in a row is where the body starts - Carriage Return (ASCII 13, \r) Line Feed (ASCII 10, \n)
    return body;
}

/**
 * Handle HTTP request and send response
 */

void post_save(int fd, char *body)
{
    FILE *file = fopen("post_data.txt", "a");
    if (file != NULL)
    {
        char status[32] = "{\"status\": \"ok\"}";
        fwrite(body, sizeof(char), strlen(body), file);
        fclose(file);
        send_response(fd, "HTTP/1.1 200 OK", "application/json", status, strlen(status));
    }
}

void *handle_http_request(void *arguments)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];
    char request_type[8];
    char request_path[1024];

    struct arg_struct *args = arguments;
    int fd = args->arg1;
    struct cache *cache = args->arg2;

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0)
    {
        perror("recv");
        pthread_exit(NULL);
    }

    sscanf(request, "%s %s", request_type, request_path);

    if (strcmp(request_type, "GET") == 0)
    {
        if (strcmp(request_path, "/d20") == 0)
        {
            get_d20(fd);
        }
        else
        {
            get_file(fd, cache, request_path);
        }
    }
    else if (strcmp(request_type, "POST") == 0)
    {
        if (strcmp(request_path, "/save") == 0)
        {
            post_save(fd, find_start_of_body(request));
        }
    }
    pthread_exit(NULL);
}

/**
 * Main
 */
int main(void)
{
    int newfd;                          // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];
    pthread_t thread_id;

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
    // forks a handler process to take care of it. The main parent
    // process then goes back to waiting for new connections.

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

        struct arg_struct args;
        args.arg1 = newfd;
        args.arg2 = cache;

        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.
        pthread_create(&thread_id, NULL, handle_http_request, &args);
        pthread_join(thread_id, NULL);
        // handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}
