C web server notes
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
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>
# all included C libraries in server.c file

# setup
setup a port and backlog with #define
# void sigchild_hander() function
takes in an int as param
saves and errno
uses a waitpid.
sigchild_handler is for handling zombie processes when a child process dies
this is only used when there is multi-processing implemented on the server

# void start_reaper(void)
takes in void as param.
reaps all dead processes

# void get_in_addr(struct sockaddr *sa*)
gets an internet adress, either IPv4 or IPv6
helper function to make printing easier

# int get_listener_socket(char *port*)
main listening socket. 
looks at local network interfaces and tries to find some that match our requirements
once we ahve a list of potential interafaces, loop through them and try to set up a socket on each. Quit looping the firs time we have success.
... this goes on for a while. code is documented look there.

# int send_response(int fd, char *header, char *content_type, char *body)
our HTTP response functionality.


# handle_http_request()
parse first line of the HTTP request header to see if this is a GET  or POST
request and to see what path it is. Use this information to decide which 
handler function to call
# request 
this variable holds the entire http request once the recv() call returns
then we read three components from the first line of the HTTP header. 
use sscanf() for this.
right after that, call the appropriate handler based on the request type 
nad the path ("/", '/d20' ect) 
start by checking for '/' and add the others later as you get to them
the handler for GET '/' is "get_root()", other names for other routes are self explanitory.
us strcmp() for matching the request method and path. strcmp() returns 0 if the strings are the same.
make sure to include resp_404(), for when user tries to hit route that doesnt exist.

# implment get_root() handler
this will call send_response()
look at resp_404 to see usage of what send_response() should look like.


# implement send_response()
this needs to build a complete HTTP response with the given paramenters. 
It should write the response to the string in the 'response' variable.
the total length of the header and body should be stored in the 'response_length' variable
so that the 'send()' call knows how many bytes to send out over the wire.
use sprintf() for creating the HTTP response. strlen() for computing content length. sprintf also
returns the total number of bytes in the resulting string, which might be helpful.
# implement get_d20
# implement get_date





















