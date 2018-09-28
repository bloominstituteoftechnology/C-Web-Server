# A Simple Web Server in C

In this project, we'll finish the implementation of a web server in C.

What you need to write:

* HTTP request parser
* HTTP response builder
* LRU cache
  * Doubly linked list (some functionality provided)
  * Use existing hashtable functionality (below)

* Your code will interface with the existing code. Understanding the existing
  code is an expected part of this challenge.

What's already there:

* Low-level networking code
* Code that determines a MIME type from a file extension
* File-reading code
* A hashtable implementation
* A linked list implementation (used solely by the hashable--you don't need it)
* All system calls, including `send()` and `recv()`

## What is a Web Server?

A web server is a piece of software that accepts HTTP requests (e.g. GET
requests for HTML pages), and returns responses (e.g. HTML pages). Other common
uses are GET requests for getting data from RESTful API endpoints, images within
web pages, and POST requests to upload data to the server (e.g. a form
submission or file upload).

## Reading

* [Networking Background](guides/net.md)
* [Doubly-Linked Lists](guides/dllist.md)
* [LRU Caches](guides/lrucache.md)
* [MIME types](guides/mime.md)

## Assignment

We will write a simple web server that returns files and some specialized data
on a certain endpoint.

* `http://localhost:3490/d20` should return a random number between 1 and 20
  inclusive as `text/plain` data.
* Any other URL should map to the `serverroot` directory and files that lie
  within. For example:

  ```
  http://localhost:3490/index.html
  ```

  should serve the file

  ```
  ./serverroot/index.html
  ```

Examine the skeleton source code in `server.c` and `cache.c` for which pieces
you'll need to implement.

**IMPORTANT** _Spend some time inventorying the code to see what is where. Write
down notes. Write an outline. Note which functions call which other functions.
Time spent up front doing this will reduce overall time spent down the road._

_The existing code is all one big hint on how to attack the problem._

For the portions that are already written, study the moderately-well-commented
code to see how it works.

There is a `Makefile` provided. On the command line, type `make` to build the
server.

Type `./server` to run the server.

### Main Goals

_Read through all the main and stretch goals before writing any code to get an
overall view, then come back to goal #1 and dig in._

#### Days 1 and 2

1. Examine `handle_http_request()` in the file `server.c`.

   You'll want to parse the first line of the HTTP request header to see if this
   is a `GET` or `POST` request, and to see what the path is. You'll use this
   information to decide which handler function to call.

   The variable `request` in `handle_http_request()` holds the entire HTTP
   request once the `recv()` call returns.

   Read the three components from the first line of the HTTP header. Hint:
   `sscanf()`.

   Right after that, call the appropriate handler based on the request type
   (`GET`, `POST`) and the path (`/d20` or other file path.) You can start by
   just checking for `/d20` and then add arbitrary files later.

   Hint: `strcmp()` for matching the request method and path. Another hint:
   `strcmp()` returns `0` if the strings are the _same_!

   > Note: you can't `switch()` on strings in C since it will compare the string
   > pointer values instead of the string contents. You have to use an
   > `if`-`else` block with `strcmp()` to get the job done.

   If you can't find an appropriate handler, call `resp_404()` instead to give
   them a "404 Not Found" response.

2. Implement the `get_d20()` handler. This will call `send_response()`.

   See above at the beginning of the assignment for what `get_d20()` should
   pass to `send_response()`.

   If you need a hint as to what the `send_response()` call should look like,
   check out the usage of it in `resp_404()`, just above there.

   Note that unlike the other responses that send back file contents, the `d20`
   endpoint will simply compute a random number and send it back. It does not
   read the number from a file.

   > The `fd` variable that is passed widely around to all the functions holds a
   > _file descriptor_. It's just a number use to represent an open
   > communications path. Usually they point to regular files on disk, but in
   > the case it points to an open _socket_ network connection. All of the code
   > to create and use `fd` has been written already, but we still need to pass
   > it around to the points it is used.

3. Implement `send_response()`.

   This needs to build a complete HTTP response with the given parameters. It
   should write the response to the string in the `response` variable.
   
   The total length of the header **and** body should be stored in the
   `response_length` variable so that the `send()` call knows how many bytes to
   send out over the wire.

   See the [HTTP](#http) section above for an example of an HTTP response and
   use that to build your own.

   Hint: `sprintf()` for creating the HTTP response. `strlen()` for computing
   content length. `sprintf()` also returns the total number of bytes in the
   result string, which might be helpful.

   > The HTTP `Content-Length` header only includes the length of the body, not
   > the header. But the `response_length` variable used by `send()` is the
   > total length of both header and body.

4. Implement arbitrary file serving.

   Any other URL should map to the `serverroot` directory and files that lie
   within. For example:

   `http://localhost:3490/index.html` serves file `./serverroot/index.html`.

   `http://localhost:3490/foo/bar/baz.html` serves file
   `./serverroot/foo/bar/baz.html`.

   You might make use of the functionality in `file.c` to make this happen.

   You also need to set the `Content-Type` header depending on what data is in
   the file. `mime.c` has useful functionality for this.

#### Days 3 and 4

Implement an LRU cache. This will be used to cache files in RAM so you don't
have to load them through the OS.

When a file is requested, the cache should be checked to see if it is there.
If so, the file is served from the cache. If not, the file is loaded from
disk, served, and saved to the cache.

The cache has a maximum number of entries. If it has more entries than the
max, the least-recently used entries are discarded.

The cache consists of a [doubly-linked
list](https://en.wikipedia.org/wiki/Doubly_linked_list) and a
[hash table](https://en.wikipedia.org/wiki/Hash_table).

The hashtable code is already written and can be found in `hashtable.c`.

1. Add cache entries to `cache.h`.

   A cache entry should contain everything needed to serve the file:

   * Endpoint path (e.g. `"/foo/bar.html"`)
   * Content length (e.g. `2123`)
   * Content type (e.g. `"text/html"`)
   * Content itself (e.g. `"<html><head>...etc."`)

   The strings should be of type `char *` (as opposed to arrays). We'll allocate
   the space for them later.

   In addition, since it's a doubly-linked list, the cache entry should have:

   * Prev and next pointers to cache entries.

2. Implement `cache_put()` in `cache.c`.

   Algorithm:

   * Allocate a new cache entry with the passed parameters.
   * Insert the entry at the head of the doubly-linked list.
   * Store it in the hash table keyed by `path`.
   * Increment the current cache size.
   * If the cache size is greater than max size:
     * Remove the cache entry at the tail of the linked list.
     * For the path in that cache entry, delete the item from the hash table.
     * Free the cache entry.

3. Implement `cache_get()` in `cache.c`.

   Algorithm:

   * Attempt to find the cache entry pointer by `path` in the hash table.
   * If not found, return `NULL`.
   * Move the cache entry to the head of the doubly-linked list.
   * Return the cache entry pointer.

4. Add caching functionality to `server.c`.

   When a file is requested, first check to see if the path to the file is in
   the cache. (Use the file path as the key.)

   If it's there, serve it back.

   If it's not there:

   * Load the file from disk (see `file.c`)
   * Store it in the cache
   * Serve it

### Stretch Goals

#### Automatic `index.html` serving

We know that if the user hits `http://localhost:3490/index.html` it should
return the file at `./serverroot/index.html`.

Make it so that if the user hits `http://localhost:3490/` (which is endpoint
`/`, on disk `./serverroot/`), if no file is found there, try adding an
`index.html` to the end of the path and trying again.

So `http://localhost:3490/` would first try:

```
./serverroot/
```

fail to find a file there, then try:

```
./serverroot/index.html
```

and succeed.

#### Expire cache entries

It doesn't make sense to cache things forever--what if the file changes on disk?

Add a `created_at` timestamp to cache entries.

If an item is found in the cache, check to see if it is more than 1 minute old.
If it is, delete it from the cache, then load the new one from disk as if it
weren't found.

You'll have to add `cache_delete` functionality to your cache code.

#### Post a file:

1. Implement `find_start_of_body()` to locate the start of the HTTP request body
   (just after the header).

2. Implement the `post_save()` handler. Modify the main loop to pass the body
   into it. Have this handler write the file to disk. Hint: `open()`, `write()`,
   `close()`. `fopen()`, `fwrite()`, and `fclose()` variants can also be used,
   but the former three functions will be slightly more straightforward to use
   in this case.

   The response from `post_save()` should be of type `application/json` and
   should be `{"status":"ok"}`.

#### Concurrency

_Difficulty: Pretty Dang Tough_

Research the pthreads library.

When a new connection comes in, launch a thread to handle it.

Be sure to lock the cache when a thread accesses it so the threads don't step on
each other's toes and corrupt the cache.

Also have thread cleanup handlers to handle threads that have died.

