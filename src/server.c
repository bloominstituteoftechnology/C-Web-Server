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
    int response_length = sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n%s",content_type, content_length,body);
    int rv = send(fd, response, response_length, 0);

    if (rv < 0)
    {
        perror("send");
    }
    return rv;
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



void print_header(http_header *req_header)
{
    printf("HTTP Request \n");
    printf("========= \n ");
    printf("[%s] Request come from  [%s] for url=[%s] ", req_header->method,req_header->Host, req_header->url);
    printf("\n========= \n ");
}

/**
 * Search for the end of the HTTP header
 *
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 */

int http_header_parser(http_header *req_header, char *request)
{

    // http_extract_header();
    char *pstr1 = request, *token[50] = {NULL, NULL, NULL, NULL, NULL};
    // printf("request in parser = %s ", request);
    int retval = SUCCESS;
    char *saveptr1 = NULL, *saveptr2 = NULL;
    int j;
    for (j = 0; j < 50; j++, pstr1 = NULL)
    {
        token[j] = strtok_r(pstr1, "\r\n", &saveptr1);
        if (token[j] == NULL)
            break;
        // printf(" GOT:: j:%d: %s \n", j, token[j]);
        char *tag = strtok_r(token[j], "\": \"", &saveptr2);
        char *value = strtok_r(NULL, "\": \"", &saveptr2);
        // if host found :: put all hostwith :
        if (!strcmp(tag, "Host"))
            strcpy(req_header->Host, value);
        else if (!strcmp(tag, "Content-Type"))
            strcpy(req_header->Content_Type, value); // store type
        else if (!strcmp(tag, "POST") || !strcmp(tag, "GET"))
        {
            strcpy(req_header->method, tag);
            strcpy(req_header->url, value);
        }
        else if (!strcmp(tag, "User-Agent"))
            strcpy(req_header->User_Agent, value);
        else if (!strcmp(tag, "Accept"))
            strcpy(req_header->Accept, value);
        else if (!strcmp(tag, "Connection"))
            strcpy(req_header->Connection , value);
        else if (!strcmp(tag, "Referer"))
            strcpy(req_header->Referer , value);
        else if (!strcmp(tag, "Accept-Encoding"))
            strcpy(req_header-> Accept_Encoding, value);
        else if (!strcmp(tag, "Upgrade-Insecure-Requests"))
            strcpy(req_header-> Upgrade_Insecure_Request, value);
        else if (!strcmp(tag, "Content-Length"))
            strcpy(req_header->Content_Length, value); // store content-length //after that data start

        // after getting content length // after that data start
        // printf(" GOT_TAG:: j:%d - -> tag = [%s] value = [%s]   \n", j, tag, value);
    }
    return retval;
}

int http_body_parser(char *req_body, char *request)
{
    int retval = SUCCESS;
    req_body = strstr(request, "\r\n\r\n"); // this will give data
    printf("req_body=[%s]\n", req_body);
    if (req_body != NULL)
        return retval;
    // else
    //     return FAILURE
}
int allowed_files(char *req_url);

int get_file_content(char *body, char *req_url, int *filesize)
{
    int retVal = SUCCESS;
    FILE *ptr;
    char ch;

    // Opening file in reading mode

    printf("requested file = [%s] \n", req_url);
    char url[100];
    char public_directory[20] = "serverroot/"; // define in config file //TODO
    sprintf(url, "%s%s", public_directory, req_url);
    ptr = fopen(url, "rb");

    if (NULL == ptr)
    {
        printf("file can't be opened \n");
        return FAILURE;
    }

    printf("content of this file are \n");

    size_t file_size;
    size_t ret;
    fseek(ptr, 0, SEEK_END);
    file_size = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);
    printf("File size: %d\n", file_size);

    if (!file_size)
    {
        printf("Warring! Empty input file!\n");
    }
    else if (file_size >= BUFFER_SIZE)
    {
        printf("Warring! File size greater than %d. File will be truncated!\n", BUFFER_SIZE);
        file_size = BUFFER_SIZE;
    }
    ret = fread(body, sizeof(char), file_size, ptr);
    if (file_size != ret)
        printf("I/O error\n");
    else
    {
        printf("FILE %s Read successfully ", req_url);
        *filesize = file_size;
    }

    // Closing the file
    fclose(ptr);

    return SUCCESS;
}
/**
 * Read and return a file from disk or cache
 */
int get_requested_static_file(int fd, char *body, char *req_url)
{

    size_t filesize;
    if (SUCCESS == get_file_content(body, req_url, &filesize))
    {
        // printf("body=[%s] file size= %d ", body, filesize);
        // set_http_response_header());
        char buffer[65000];
        int size=(int*)filesize;
        
        char header[50],content_type[30];
        sprintf(header,"http/1.1 200 OK");
        sprintf(content_type,"text/html");
        // printf(" type  = %d ", size);
        send_response(fd,header,content_type,body,size);
        // if (send(fd, buffer, length, 0) != length)
        //     printf("error in seding");
        // else
        //     printf("sending ");
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}
int handle_http_get_request(int fd, char *req_url)
{
    // handle static file request
    http_header *response_header;
    response_header = (struct http_header *)malloc(sizeof(http_header)); // free this header after sending
    char body[65000];


    if (MATCH(req_url, "/index.html") || MATCH(req_url, "/") || MATCH(req_url, "/about.html"))
    {
        req_url++; // for removing /
        if (SUCCESS == get_requested_static_file(fd, body, req_url))
        {
            printf("success ");
        }
    }
    else
    {
        printf("INVALID URL = [%s] \n", req_url);
        // send404 //TODO
        return FAILURE;
    }
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
    // printf("========\n%s\n===========\n", request);

    // extracting the components
    http_header *req_header; // use pointer //malloc and dealloc after sending response

    req_header = (struct http_header *)malloc(sizeof(http_header));
    char req[65000];
    char *req_body;
    strcpy(req, request);


    //parsing header
    if (http_header_parser(req_header, req) == FAILURE)
        printf("Error in Parsing http request");

    print_header(req_header);

    //parsing body
    if (http_body_parser(req_body, request) == FAILURE)
        printf("Error in Parsing http body");

    //valide http request
    // if(validate_http_request()==FAILURE)
    // {
    // check user login
    // send to user_login
    // else do nothing
    // }

    // printf("Got method = %s ", )
    if (MATCH(req_header->method, "GET"))
    {
        printf("get");
        if (SUCCESS == handle_http_get_request(fd, req_header->url))
            printf("handle_http_get_request SUCCESS\n");
        else
            printf("Failure");
    }
    else if (MATCH(req_header->method, "POST"))
    {
        printf("post \n");
        // handle_http_post_request(req_header->url, req_body);
    }


    free(req_header);

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
