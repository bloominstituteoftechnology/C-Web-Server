# Web Server

Software that: 
- accepts HTTP requests (e.g. GET requests for HTML pages), and 
- returns responses (e.g. HTML pages). 
Examples: 
- GET requests for getting data from RESTful API endpoints, images within web pages, and 
- POST requests to upload data to the server (e.g. a form submission or file upload).
- PUT
- DELETE
- HEAD same as GET, but does not return anything in the message body. The returned metadata should be identical to info returned in a GET request.
- ping 1.1.1.1 (Internet's fastest DNS (Domain Name System) address )
- datatrace -n 1.1.1.1  traceroute 1.1.1.1

## Networking Protocols
Protocol: established code of procedures between client and servers handling requests and responses.

Data packets are encapulated from the ground up (starting with the HTTP header and ending with the Ethernet Header)

### Network Stack / Data Encapulation Hierarchy 
|-----------------------|
|    Ethernet Header    | Routing on the LAN        Physical/Data ling
|  Network Access Layer |                           Access physical media
|-----------------------|
|  IP (Address) Header  | Routing on the Internet   How Data Is Transferred
| Network/Internet Layer|                           Defines datagram/               
|-----------------------|
|   TCP Header / UDP    | Data integrity            Reliability
|    Transport Layer    |                           End-to-end deliver
|-----------------------|
|      HTTP Header      | Deals with web data       Formats req & resp
|   Application Layer   |                           
|-----------------------|
| <h1>Hello World!</h1> | Actual data
|_______________________|

### Ethernet
Sends data and listens at the same time to make sure it is identical. If it isn't it means someone else is sending at the same time and the data will become corrupt. So, it wait 7.6 + random seconds and resends. If too many try sending at the same time the ethernet fails/overload (packet storm).

### TCP Protocol (Transmission Control Protocol)
- Reliable
- Uses Three-Way Handshake
- Breaks packet into smaller packets

Client/Initiator                                    Server / Listener
----------------------------------                  -----------------------------------------
connect()                                           listen()
                                   -----SYN----->   TCB initialized to SYN-RECEIVED STATE

Success code returned by connect() <---SYN-ACK---

                                   -----ACK----->   TCB transitions to Established State^^^

                        Completion: Data packets exchanged

^^^ If the ACK was never recieved the server has another set for losts packets.

### UDP (User Datagram Protocol) Protocol (vs TCP)
- Not as reliable
- Less protocols and more lost packets
- Ability to send significantly more packets than TCP
- No handshake or acknowledgement



## Unix Sockets
- Higher level example of a socket is socket.io API
- Low level sockets API is what this project will implement
    - supports both TCP & UDP and IPv4 and IPv6

### Files in Unix: "Everything in Unix is a file."
File Descriptors (fd): Integer value associated with an open file.
FILE *fp = fopen("text.txt", "w");
printf("%d, fp);    // this would print an integer value

File Descriptor       Global File Table             Inode Table
---------------    -----------------------      -------------------
0                  read-only, offset: 0
1                  write-only, offset: 0        /dev/pts22
2
3                  read-write, offset: 12       /path/myfile.txt
4                  read-write, offset: 8        /path/myfile3.txt

Caveat: File can refer to:
- network connection  (shared file)
- pipe
- terminal
- actual file

### Unix-Style Sockets API System Calls

Client
---------
socket()
connect()
write()
read()

Server
---------
socket()
bind()              net.c 79
listen()            net.c 100
accept()
read()
write()

- socket() creates a new socket (the fd is stored to an int)
- bind() bind IP address (associated with a network card)
- send() write data
- recv() read data


#### Initializing a Socket

```c
#include <sys/socket.h>
int sockfd = socket (...)   // initialize a socket (file descriptor)
send(sockfd, response, responseLength, 0);    // 0 is a flag
recv(sockfd, request, requestBufferSize - 1, 0); // 0 is a flag
```

P1                          P2
   _____             ______
fd _____ shared file ______ fd



Services by Port Number
- 7:    echo
- 21:   ftp (File Transfer Protocol)
- 23:   telnet (TCP connection with a server & issue raw HTTP requests)
        telnet www.example.com 80
- 25:   smtp (Simple Mail Transfer)
- 80:   http / www
- 6000: x11 (X Window System -- a unix based gui)




## HTTP Protocol
- HTTP responsible for formatting requests and responses between client and servers
- HTTP does *not* handle how data is transfered.
- HTTP does *not* hand reliability

### How to Format HTTP Packets

#### Format of a GET Request
Syntax:
HTTP Method /URL  Protocol/Version          ** /URL w/o .com
Host: Host of target URL
Blank line indicating the end of the Request use \n (even if there is no body a blank line is required)

Example:
GET /example HTTP/1.1
Host: lambdaschool.com
Connection: close
X-Header: whatever


GET /pub/WWW/TheProject.html HTTP/1.1
Host: www.w3.org


#### Format of a POST Request
Syntax:
POST request-URI HTTP-version
Content-Type: mime-type
Content-Length: number-of-bytes
(other optional request headers)

(URL-encoded query string)

Example:
POST /magazines HTTP/1.1
Host: example.gov.au
Accept: application/json, text/javascript

#### Format of a DELETE Request
DELETE /magazines HTTP/1.1
Host: example.gov.au
Accept: application/json, text/javascript

#### Format of a HEAD Request
HEAD /magazines HTTP/1.1
Host: example.gov.au
Accept: application/json, text/javascript

### Format of a Response
Syntax:
Protocol/Version Status Code
Date: Timestamp when response was created
Connection: close     (set to open if more info will be sent)
Content-Length: number of bytes in the body (w/o blank line)
Content-Type: MIME (Multipurpose Internet Mail Extensions) type of the content of the body to be rendered  (text/html, text/plain, text/css, image/jpg, image/gif, image/png, application/json, application/javascript, multipart/form-data)

<html><head><title>Data being sent back</title></head></html>

#### Example: 200 Response
HTTP/1.1 200 OK
Date: Wed Dec 20 13:05:11 PST 2017
Connection: close
Content-Length: 12345
Content-Type: text/html

<html><head><title>Lambda</title></head></html>

#### Example: 404 Response
HTTP/1.1 404 Not Found
Date: Wed Dec 20 13:05:11 PST 2017
Connection: close
Content-Length: 13
Content-Type: text/plain

404 Not Found

send_response() response_length send() sprintf() strlen()  sprintf()  time() localtime() time.h


## MISC

### sscanf & spacing
- useful for parsing incoming request from a web browser
- sscanf is scanf for strings
- Request and responses have specific spacing
    - e.g.: GET /example HTTP/1.1
            HTTP/1.1 404 Not Found
- %20 is the ASCII Hex code for space. Used for space in URL (no additional spaces allowed)
- JS equivalent to .split(" ")
- C compiler will concatenate char * that are next to each other, e.g., char *s = "my" "string"

```c
#include <stdio.h>

/*
GET /foobar HTTP/1.1
Host: www.example.com
Connection: closecon
X-Header: whatever
*/

int main(void)
{
    // s holds the HTTP request
    char *s = "GET /foobar HTTP/1.1\nHost: www.example.com\nConnection: close\nX-Header: whatever";

    char method[200];
    char path[8192];

    sscanf(s, "%s %s", method, path);

    printf("method: \"%s\"\n", method);
    printf("path: \"%s\"\n", path);
}
```

### sprintf & Content Length
- useful for building outgoing response to the server
- use char *body to save body.
- int length = strlen(body);
- use %s

```c
#include <stdio.h>
#include <string.h>
​
int main(void)
{
    char response[500000];
​
    char *body = "<h1>Hello, world!</h1>";
    int length = strlen(body);
​
    sprintf(response, "HTTP/1.1 200 OK\n"
            "Content-Type: text/html\n"
            "Content-Length: %d\n"
            "Connection: close\n"
            "\n"
            "%s",
            length, body
        );
​
    printf("%s", response); // send()
}
```

## Cache

Get Requests
GET /index.html
GET /index.html
GET /style.css
GET /foo.gif
GET /style.css
GET /index.html

Cache(3)        Disk



head               tail

bar -> foo -> style
style -> bar -> foo

|||||||HASHTABLE|||||||
index <-> style <-> bar

Add a cache miss to head of list O(1)
Remove LRU from end of list O(1)
Remove LRU from hashtable O(1)

Item in cache? O(1)
