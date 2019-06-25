#include <stdio.h>

/*
GET /foobar HTTP/1.1
Host: www.example.com
Connection: close
X-Header: whatever
*/

int main(void)
{
	char *request = 
		"GET /foobar HTTP/1.1\n"
		"Host: www.example.com\n"
		"Connection: close\n"
		"X-Header: whatever\n"
		"\n";  // request ends in blank line

	char method[128]; // "GET"
	char path[8192];  // "/foobar"

	sscanf(request, "%s %s", method, path);

	printf("%s\n", method); // "GET"
	printf("%s\n", path);   // "/foobar"
}


#include <stdio.h>
#include <string.h>

int main(void)
{
	char response[500000];

	char *body = "<h1>Hello, world!</h1>";
	int len = strlen(body);

	sprintf(response,
		"HTTP/1.1 200 OK\n"
		"Content-Type: text/html\n"
		"Content-Length: %d\n"
		"Connection: close\n"
		"\n"
		"%s",
		len, body);

	// call send(response)

}