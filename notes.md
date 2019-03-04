# Notes about Project

## Send response

-   build complete response with the params into the response variable
-   total length of the header and body is stored in response_length, so send() knows, this is not the same as the content-length
-   sprintf() for creating response, also returns the total number of bytes is string
-   strlen for content length
-   look at time() & localtime() for the date (already included in time.h)
-   to make sure working, call resp_404 in the main
