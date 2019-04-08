#include <stdio.h>
#include <string.h>

/*
    HTTP/1.1 200 OK\n
    Date: .... <--- would be here but not in this example
    Content-Type: text/html\n
    Content-Length: %d\n
    Connection: close\n
    \n <--- needs to be a new line that separates header from body
    <Body goes here>
*/

// Example of how to build a response
int main(void)
{
    // buffer to hold the res data
    char response[500000];

    char *body = "<h1>Hello, world!</h1>";
    int length = strlen(body);

    // Let's build the actual response now
    sprintf(response,
            "HTTP/1.1 200 OK\n"
            "Content-Type: text/html\n"
            "Content-Length: %d\n"
            "Connection: close\n"
            "\n"
            "%s",
            length, body);

    printf("%s", response);
    return 0;
}