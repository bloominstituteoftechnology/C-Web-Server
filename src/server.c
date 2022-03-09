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

int append_str(char *target, char *src, int idx)
{
    for (int j = 0; j < src[j] != NULL; idx++, j++)
    {
        target[idx] = src[j];
    }

    return idx;
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

    // Build HTTP response and store it in response

    char tmpheader[30];
    for (int i = 0; header[i] != NULL; i++)
    {
        tmpheader[i] = header[i];
    }

    char *token = strtok(tmpheader, " ");

    char *http_version = token;
    token = strtok(NULL, " ");

    char *status = token;
    token = strtok(NULL, " ");

    char status_code[20];
    int si = 0;
    for (int i = 0; token[i] != NULL; i++)
    {
        status_code[si++] = token[i];
    }

    if (strcmp(status_code, "NOT") == 0)
    {
        token = strtok(NULL, " ");
        status_code[si++] = ' ';
        for (int i = 0; token[i] != NULL; i++)
        {
            status_code[si++] = token[i];
        }
    }

    int idx = 0;

    // http version
    idx = append_str(response, http_version, idx);
    response[idx++] = ' ';

    // status
    idx = append_str(response, status, idx);
    response[idx++] = ' ';

    // status code
    idx = append_str(response, status_code, idx);
    response[idx++] = '\n';

    // date is optional
    time_t now = time(0);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';

    idx = append_str(response, "Date: ", idx);
    idx = append_str(response, time_str, idx);
    response[idx++] = '\n';

    // connection status
    char *connection_status = "Connection: close";
    idx = append_str(response, connection_status, idx);
    response[idx++] = '\n';

    // content(body) length
    char content_length_key[20] = "Content-Length: ";
    char content_length_str[20];
    sprintf(content_length_str, "%d", content_length); // int -> str

    idx = append_str(response, content_length_key, idx);
    idx = append_str(response, content_length_str, idx);
    response[idx++] = '\n';

    // content-type
    char content_type_total[30] = "Content-Type: ";
    strcat(content_type_total, content_type);

    idx = append_str(response, content_type_total, idx);
    // idx += snprintf(response + idx, strlen(content_type_total), "%s\n", content_type_total); // snprintf로 다 바꾸고 싶은데 오류나네..

    response[idx++] = '\n';
    response[idx++] = '\n';

    // body
    // idx = append_str(response, body, idx);
    printf("body = %s\n", body);
    idx += snprintf(response + idx, strlen(body), "%s\n", body);

    // debug response
    printf("================ response ===============\n");
    for (int j = 0; j < idx; j++)
    {
        printf("%c", response[j]);
    }
    printf("\n");
    printf("=========================================\n");

    // unsigned int response_length = strlen(header) + strlen(body);
    unsigned int response_length = idx;

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

    int random = 1 + rand() % 20;
    printf("generated random number = %d\n", random);

    char random_number[10];
    sprintf(random_number, "%d", random); // int -> str
    send_response(fd, "HTTP/1.1 201 CREATED", "text/plain", random_number, 50);
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

    printf("in resp_404(), filepath = %s\n", filepath);

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

    // Fetch the {filename}.html file

    snprintf(filepath, sizeof filepath, "%s", SERVER_FILES);
    strcat(filepath, request_path);

    printf("in get_file(), filepath = %s\n", filepath);

    filedata = file_load(filepath);

    if (filedata == NULL)
    {
        // TODO: make this non-fatal
        // fprintf(stderr, "cannot find system %s file\n", filepath);
        resp_404(fd);
        return;
        // exit(3);
    }

    mime_type = mime_type_get(filepath);

    printf("in get_file(), filepath = %s\n", filepath);

    send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);

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

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0)
    {
        perror("recv");
        return;
    }

    printf("============= request ==============\n");
    printf(request);
    printf("====================================\n");

    char request_type[30];
    char path[100];

    // Read the first two components of the first line of the request
    sscanf(request, "%s %s", request_type, path);

    printf("%s %s\n", request_type, path);

    // If GET, handle the get endpoints
    if (strcmp(request_type, "GET") == 0)
    {
        printf("get request\n");
        printf("request path = %s\n", path);

        if (strcmp(path, "/d20") == 0)
        {
            get_d20(fd);
        }
        else
        {
            // resp_404(fd);
            get_file(fd, NULL, path);
        }
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

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}
