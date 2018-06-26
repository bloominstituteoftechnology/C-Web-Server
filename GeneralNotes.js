// Port is 3490 with `PORT`

// Line 180 in server.c sends 404

// (1) Assignment
// line 265 is main handler
// line 292 - sscanf() the request and determine if the first argument is 
//            a post, get, put, delete
//          - call the CRUD handler passing the second index of the header as the path

// The handler for GET / is get_root() (search for the skeleton code). 
// The handler for GET /d20 is get_d20().
// The handler for GET /date is get_date().

// use strcmp() for matching the request method and path. 
//         - strcmp() returns 0 if the strings are the same
//            - if else block

// If you can't find an appropriate handler, 
// call resp_404() instead to give them a "404 Not Found" response.

// (2) Assignment
// implement get_root() - pass <h1> Hello world! </h1>
//  the send_response() call look likes, check out the usage of it in resp_404()


// The fd variable that is passed widely around to all the functions holds a file descriptor. 
// It's just a number use to represent an open communications path. 
// Usually they point to regular files on disk, but in the case it points to an 
// open socket network connection. All of the code to create and use fd
//  has been written already, but we still need to pass it around to the points


// (3) Assignment
// implement send_response().
//   should build a complete HTTP response and send it to the response variable
// total length of header and body needs to be calculated and held in `response_length`
// // HTTP/1.1 200 OK
// Date: Wed Dec 20 13:05:11 PST 2017
// Connection: close
// Content-Length: 41749
// Content-Type: text/html
// <!DOCTYPE html><html><head><title>Lambda School ...

// use sprintf() for creating the HTTP response. 
// strlen() for computing content length. 
// sprintf() also returns the total number of bytes in the result string

// The HTTP Content-Length header only includes the length of the body, not the header

// (4) Assignment
// Implement the get_d20() handler. Hint: srand() with time(NULL), rand()

// (5) Assignment
// Implement the get_date() handler. Hint: time(NULL), gmtime().


