#include <stdio.h>

/*
    given this request, how do we parse (extract important parts of it) it?
    GET /foobar HTTP/1.1
    Host: www.example.com
    Connection: close

*/

int main(void)
{
    char *s = "GET /foobar HTTP/1.1\nHost: www.example.com\nConnection: close\n";

    // buffer to hold the method
    char method[200];
    // buffer to hold the path
    char path[8192];

    // take some string, supply a format specifier
    // %s %s pulls out the first two strings which will be the GET and path
    sscanf(s, "%s %s", method, path);

    printf("method: %s\n", method);
    printf("path: %s\n", path);
    return 0;
}