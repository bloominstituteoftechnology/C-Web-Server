# Web Server FAQ

## Contents

### Troubleshooting

* [I rebuilt and re-ran the server, but nothing's happening.](#q300)
* [After sending the request and getting the response, my client is just sitting there, waiting, and not closing the connection.](#q600)
* [After sending a `GET` request, the server is just sitting there waiting for something and not responding.](#q700)
* [I tried to do the `telnet` demo, but it instantly comes back with a 501 response.](#q1500)
* [I added the `Date` HTTP header using `asctime()` (or `ctime()`) but now everything shows up in my browser as plain text and some of the headers are in the body, as well.](#q3100)
* [I'm trying to test with `curl` or the browser, and it's telling me "`Connection refused`". Why?](#q4100)
* [My server seems to hang indefinitely on the line `newfd = accept(listenfd, ...)` in the `main` function. It never moves past it when requests are made to it. What's going on?](#q4200)
* [When serving a file that is in the cache, the browser renders some garbage text instead of/in addition to what is expected to be rendered.](#q4500)

### General

* [How can I use `curl` on the command line to test my server?](#q800)
* [How do browsers handle caches?](#q900)
* [Our server only sends out one file at a time. How do browsers get multiple files, like if `index.html` refers to `styles.css`?](#q1300)
* [How can I fix my server to return binary data as well as text data? I want to get `cat.png` back to the browser.](#q1400)
* [What is an "octet"?](#q2300)
* [What's a normal rate of packet loss?](#q3600)
* [Do packets get delivered in the order they're sent or do they get jumbled up by the process?](#q3800)
* [Can different packets from the same request can take a different routes to stop bottle necks?](#q3900)
* [If your request hits a load balancer will it remember such that each subsequent request you send will be sent to the same server?](#q4000)
* [What's the difference between `sprintf` and `snprintf`? When do I use one over the other?](#q4300)

### Sockets

* [What C libraries do we need for working with sockets?](#q2600)
* [Do other higher-level languages use sockets under the hood for network communication?](#q3200)

### TCP/UDP/IP, Network Stack

* [What are the layers of the network stack?](#q1900)
* [How do you know which layer of the network stack is causing a problem?](#q2000)
* [What's the main difference between TCP and UDP, and when to use each?](#q1600)
* [Does the "T" in TCP stand for Transport or Transmission?](#q1700)
* [How are UDP packets lost?](#q1800)
* [Will we work with both UDP and TCP on this project or just TCP?](#q2500)
* [In TCP, what is the Transmission Control Block (TCB)?](#q3300)
* [Does UDP ever send the same data multiple times?](#q3400)
* [How much faster is UDP than TCP? How can I measure it?](#q3500)
* [What's a normal rate of packet loss?](#q3600)
* [Does <i>most</i> of the UDP data normally arrive?](#q3700)
* [Do packets get delivered in the order they're sent or do they get jumbled up by the process?](#q3800)
* [Can different packets from the same request can take a different routes to stop bottle necks?](#q3900)
* [If your request hits a load balancer will it remember such that each subsequent request you send will be sent to the same server?](#q4000)

### HTTP

* [Why do we have to specify the `Host` field in the HTTP request? Isn't it redundant since we just connected to that host?](#q100)
* [Why is the `Content-Length` different than the number of bytes we pass to `send()`?](#q400)
* [Does the order of the fields in the HTTP header matter?](#q500)
* [Will we be diving into HTTPS or any type of secured connection protocols?](#q2400)
* [I added the `Date` HTTP header using `asctime()` (or `ctime()`) but now everything shows up in my browser as plain text and some of the headers are in the body, as well.](#q3100)
* [When I request the `cat.jpg` image, I just get a black screen rendered in my browser. What's the problem?](#q4300)

### Existing Code

* [In `send_response()`, the `body` argument is passed as a `void*`. Is there some scenario where the `body` is not a string?](#q200)
* [What does the `flags` parameter do in `send()` and `recv()`?](#q2100)
* [What is that `fd` variable?](#q2200)

### MIME

* [What is MIME? Outside of web servers, where are MIME types used?](#q1000)
* [Where can I find a complete list of MIME types?](#q1100)
* [How does MIME multipart work?](#q1200)

### LRU Cache

* [For the LRU cache, why do we need a doubly-linked list? Are there other data structures that could work?](#q2700)
* [How do we add an age to the cache entries to remove them when they get old?](#q2800)
* [What about an LFU cache (Least <i>Frequently</i> Used) instead of an LRU cache?](#q2900)
* [Would using Python's `OrderedDict` data structure be better or worse than using a doubly-linked list for an LRU cache?](#q3000)

## Questions

<a name="q100"></a>
### Why do we have to specify the `Host` field in the HTTP request? Isn't it redundant since we just connected to that host?

In the good old days, there was typically only one host per IP address, and
vice-versa. That is, if we connected to IP address, `198.51.100.20`, there would
have only been one host (e.g. `www.example.com`) associated with it.

In that case, the `Host` field would truly be redundant. (And in fact, it was
not included in the original 1.0 version of HTTP.)

But since then, we've moved to a time where a single computer with a single IP
might host hundreds or even thousands of different websites.

In such a case, the client needs to tell the server not only the file it's
interested in (e.g. `/funnycat.gif`), but also the host that it's coming from
(e.g. `cats.example.com`). After all, there might be hundreds of domains on this
server, and hundreds of `/funnycat.gif` files.

So the `Host` field becomes necessary. The client not only needs to specify the
file they're interested in, but also the domain they're expecting to find it on.

------------------------------------------------------------------------

<a name="q200"></a>
### In `send_response()`, the `body` argument is passed as a `void*`. Is there some scenario where the `body` is not a string?

In the MVP, no it's always a string.

But if you want to serve any kind of binary file (e.g. `foo.jpg`), you'll be
`send()`ing data that's _not_ a string.

This is why `send_response()` takes a `void*`--it can point to any type of data,
string or not.

------------------------------------------------------------------------

<a name="q300"></a>
### I rebuilt and re-ran the server, but nothing's happening.

You have to hit it with a web request. Either send your browser to
`http://localhost:3490/`, or run curl:

```shell
curl -D - http://localhost:3490/
```

------------------------------------------------------------------------

<a name="q400"></a>
### Why is the `Content-Length` different than the number of bytes we pass to `send()`?

It's because they refer to the size of the _payload_ at different layers in the
protocol stack.

Remember that HTTP runs on top of TCP, and TCP runs on top of IP, and IP runs on
top of Ethernet (on your LAN, anyway).

So we first put together our HTTP packet, and in there we put the size of the
HTTP data in `Content-Length`, and doesn't count the HTTP header.

But then we wrap _that entire thing_ inside TCP. (Well, the OS does it for us
when we call `send()`.) So the entirety of the HTTP data, header and body, needs
to be wrapped up in TCP and sent. So when we call `send()`, we give it that
entire length.

------------------------------------------------------------------------

<a name="q500"></a>
### Does the order of the fields in the HTTP header matter?

No. The key-value pairs can be in any order.

Caveat: the first line of the header is always something like this for requests:

```http
GET /index.html HTTP/1.1
```

and the first line of the response is always something like this:

```http
HTTP/1.1 200 OK
```

But _after_ that, with all the things like `Content-Length` and `Content-Type` and all that, those can be in any order.

Don't forget to end your header with an empty line!

------------------------------------------------------------------------

<a name="q600"></a>
### After sending the request and getting the response, my client is just sitting there, waiting, and not closing the connection.

Make sure you have the

```http
Connection: close
```

field in your header.

------------------------------------------------------------------------

<a name="q700"></a>
### After sending a `GET` request, the server is just sitting there waiting for something and not responding.

Make sure you end your request header with a blank line. That's how the server
knows the header is complete.

------------------------------------------------------------------------

<a name="q800"></a>
### How can I use `curl` on the command line to test my server?

`curl` sends web requests from the command line and prints the results on
standard output.

Just hit the URL and print the body:

```shell
curl http://localhost:3490/
```

Hit the URL and print out the response headers and the body:

```shell
curl -D - http://localhost:3490/
```

Hit the URL, print out the request headers (and body), print out the response
headers and body:

```shell
curl -v http://localhost:3490/
```

------------------------------------------------------------------------

<a name="q900"></a>
### How do browsers handle caches?

The browser-side cache (as opposed to the server-side cache that you'll be
writing) has the goal of speeding web page loads by reducing network traffic.

When loading a web page (or other piece of data), the browser first looks in its
cache on disk to see if the data is there. If it is, it can display it
immediately and the user doesn't have to wait for it to come in over the
network.

Of course, there are more details that need to be hashed out.

* Does the browser have a limit on the number of items in the cache?
* Does the browser have a limit on the amount of data in the cache?
* How does the browser refresh the cache or expire elements in the cache?
* How does the browser know which pages should and should not be cached?

Most browsers allow you to see a limit on the cache size in bytes. Old
information that exceeds that limit will be discarded.

Of course, when the browser hits the cache but finds some old data, it needs to
_refresh_ that cache entry from the server again.

Entries might be old, or maybe a web page has said it should never be cached.

A web server can offer hints to a web browser about how data should be cached.
For more information, see the [Cache-Control
header](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Cache-Control).

------------------------------------------------------------------------

<a name="q1000"></a>
### What is MIME? Outside of web servers, where are MIME types used?

The idea with MIME is that you're going to attach a piece of metadata to the
data you're sending to let the receiver know what _type_ (or kind) of data it is
so they can do the right thing with it.

In HTTP, the MIME type goes in the header in the `Content-Type` field. Examples:

```http
Content-Type: text/html
```

```http
Content-Type: application/javascript
```

```http
Content-Type: image/png
```

When the browser knows the MIME type, it can display the data in the way it
needs to be displayed. It can render HTML if it's HTML, or draw the image if
it's a PNG, etc.

MIME actually stands for _Multipurpose Internet Mail Extension_. It was
originally invented to allow email to have attachments. And it's still used in
email today.

Aside from that, there are a variety of miscellaneous uses, but web and email
cover 99.9% of all of them.

------------------------------------------------------------------------

<a name="q1100"></a>
### Where can I find a complete list of MIME types?

Since people are adding new MIME types all the time, there's not really a such
thing as a _complete_ list.

(You can make up your own MIME types. If they're not official, the second part
of the MIME type should be prefixed with `x-`, like `application/x-bzip`.)

Common MIME types can be found at [MDN's incomplete list of MIME
types](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Complete_list_of_MIME_types).

------------------------------------------------------------------------

<a name="q1200"></a>
### How does MIME multipart work?

It splits the body up into separate MIME sections, each with its own MIME type.

See Wikipedia: [MIME multipart
messages](https://en.wikipedia.org/wiki/MIME#Multipart_messages)

------------------------------------------------------------------------

<a name="q1300"></a>
### Our server only sends out one file at a time. How do browsers get multiple files, like if `index.html` refers to `styles.css`?

Turns out they do them in separate requests.

First the browser will get `index.html`. Then it parses the HTML and sees that
it has references to, for example, `styles.css` and `funnycats.gif`. It then
issues two more requests, one for each of those files.

There's a fair amount of overhead to this, so modern browsers typically make a
single TCP connection over the network, and then transmit multiple HTTP requests
over it. Conceptually, however, this still sends multiple HTTP requests, just
like above.

In our server, we specify the header

```http
Connection: close
```

which tells the browser we're hanging up after this one response. This is just
to make our code easier.

------------------------------------------------------------------------

<a name="q1400"></a>
### How can I fix my server to return binary data as well as text data? I want to get `cat.png` back to the browser.

If you are building your HTTP response, both header and body, with a single
`sprintf()`, you'll have trouble getting the body in there. This is because
`sprintf()` with `%s` prints a string, and a string ends on the first `'\0'`
character. Undoubtedly the PNG image is full of `0`s, so it stops printing short
of end of the data.

There are two options here:

1. Use two calls to `send()`. This ends up working better for large files
   because your response buffer variable only has to be large enough to hold the
   header (and not the header plus the body).

   1. Use `sprintf()` to make the header, but that's all.

   2. Call `send()` once to send the header.

   3. Call `send()` again to send the body.

2. Use `memcpy()` to append the body after the header. Make sure your response
   buffer is big enough to hold your largest file.

   1. Use `sprintf()` to make the header, but that's all.

   2. Use `memcpy()` to copy the body just after the end of the header.
      `memcpy()` copies a specified number of bytes; it doesn't stop at the
      first `0` byte.

   3. Call `send()` to send out the complete HTTP response.

------------------------------------------------------------------------

<a name="q1500"></a>
### I tried to do the `telnet` demo, but it instantly comes back with a 501 response.

Telnet can optionally send some control commands back and forth per the [telnet
protocol](https://tools.ietf.org/html/rfc854) and it seems the Windows version
of telnet does this. Try the
[WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) version.

------------------------------------------------------------------------

<a name="q1600"></a>
### What's the main difference between TCP and UDP, and when to use each?

TCP offers:

* Error-free data (nothing corrupted)
* In order data (nothing out of order)
* Non-duplicated data (no duplicate packets)
* Complete data (nothing missing)

UDP offers:

* Error-free data (nothing corrupted)

and none of the rest of it.

UDP is a simpler, lower-overhead protocol. It is used in applications where
speed is a priority and data can be lost without a problem. Skype calls are a
great example. If you lose 100 ms of content, you can often still understand the
caller. And if not, we have a protocol for fixing it at the human level ("You
broke up there--can you say that again?").

Another good UDP use case is sending player data to other players in an MMO. If
you drop a packet, you'll get the next update in 1/30th of a second, so no big
loss.

But player type-written chat data, that's something else. When you send a chat,
you expect it to arrive. TCP is much better.

------------------------------------------------------------------------

<a name="q1700"></a>
### Does the "T" in TCP stand for Transport or Transmission?

It stands for [Transmission](https://tools.ietf.org/html/rfc793).

Confusingly, it is located at the Transport layer of the [OSI layered network
model](https://en.wikipedia.org/wiki/OSI_model).

------------------------------------------------------------------------

<a name="q1800"></a>
### How are UDP packets lost?

The non-answer is that from our perspectives as programmers, it doesn't matter.
Could have been goats chewing on cables, could have been a meteorite strike,
could have been the Norse god Loki up to his usual tricks. The data was lost,
end of story.

But _how_?

One common way is that one of the computers along the route was simply
overloaded. It was busy routing packets, and one of them came in and was
ignored. Internet routers make best effort to forward packets, but they don't
guarantee it. If large numbers of packets are being lost, the network is
over-congested and needs to have some of the load lightened.

Also, if any of the packets were corrupted in transit (which could commonly be
due to radio interference on wifi or noisy copper wires), those packets are
dropped immediately.

Less frequently, the physical medium gets disrupted. Backhoes go through fiber.
Entire countries have had their internet cut off when a ship anchor dragged over
their internet backbone.

In those cases, the internet tries to heal the problem by routing around the
damage, if possible.

------------------------------------------------------------------------

<a name="q1900"></a>
### What are the layers of the network stack?

The full stack is described in [OSI layered network
model](https://en.wikipedia.org/wiki/OSI_model). But this is overkill for
understanding the model in a practical sense.

The simplified four-layer model commonly in use from a programmer perspective:

|       Layer       | Example Protocols                   |
|:-----------------:|:------------------------------------|
| Application Layer | HTTP, HTTPS, FTP, TELNET, TFTP, SSH |
|  Transport Layer  | TCP, UDP                            |
|   Network Layer   | IP                                  |
|    Link Layer     | Ethernet (wifi or wired)            |

------------------------------------------------------------------------

<a name="q2000"></a>
### How do you know which layer of the network stack is causing a problem?

It's comes down to learning how to troubleshoot network problems.

You start at the lowest layer. Do we have a link-layer (Ethernet) connection to
the other device?

If so, then you move up a layer. Do we have an IP layer connection to the
device?

For connectivity, it's almost always one of those.

TCP layer issues are rare, and are usually a consequence of issues at the IP or
Ethernet layer.

Application layer issues (HTTP, FTP, etc) tend to be program bugs, and you just
debug them normally.

------------------------------------------------------------------------

<a name="q2100"></a>
### What does the `flags` parameter do in `send()` and `recv()`?

It gives you additional control over how the data is sent. (Using `read()` or
`write()` with a socket descriptor is the same as calling `recv()` or `send()`
with the `flags` set to `0`.)

Practically speaking, the only flag that is remotely commonly used is `MSG_OOB`,
which is used to send _out of band_ data to the receiver. This is data that can
be logically thought of as being being outside the normal data stream, e.g. for
priority or exception data. The receiving side gets a signal indicating that
some out of band data has arrived and that normal processing should be
interrupted to handle it.

------------------------------------------------------------------------

<a name="q2200"></a>
### What is that `fd` variable?

`fd` is the traditional name of a variable that holds a _file descriptor_.

In our web server, this is a number that represents a network connection. (Since
a program might have several connections open at once, it needs to be able to
tell the OS which connection it wants to send data to.)

Where does it come from?

The server is normally sitting quietly waiting for new connections to arrive. It
does this by calling the `accept()` syscall, where it blocks until someone
connects. At that point `accept()` returns a new file descriptor representing
the connection. It is this file descriptor that is used in subsequent `send()`
and `recv()` calls.

Since this file descriptor represents a socket, some documentation refers to it
as a _socket descriptor_. It's the same as a file descriptor, just different
terminology.

In Unix, the old saying goes, _everything_ is a file. What this means is that if
you open a text file for reading, you get a file descriptor back and can use
that with `read()` and `write()` calls. But it also means that if you call
`socket()` or `accept()`, you get a socket descriptor back and you can use that
with `read()` and `write()` calls! Even though one is a traditional file on
disk, and the other is a socket, the OS treats them both like files.

Turns out with sockets, a `write()` is the same as a `send()` with the `flags`
parameter set to `0`. Same with `read()` and `recv()`.

------------------------------------------------------------------------

<a name="q2300"></a>
### What is an "octet"?

It's another word for _byte_, practically speaking.

Specifically, an octet is a number that is exactly 8 bits long.

Although a byte is also 8 bits long on basically all modern architectures,
historically there have been systems where a byte was a different number of
bits.

The term _octet_ removes the ambiguity.

------------------------------------------------------------------------

<a name="q2400"></a>
### Will we be diving into HTTPS or any type of secured connection protocols?

Not in this class.

HTTPS runs HTTP on top of another protocol called
[TLS](https://en.wikipedia.org/wiki/Transport_Layer_Security). There are [a
number of TLS implementations you can choose
from](https://en.wikipedia.org/wiki/Comparison_of_TLS_implementations).

HTTP works the same in both HTTP and HTTPS; with the latter, the TLS layer is
responsible for the encryption.

But the details of how that is done is beyond the scope of the class.

------------------------------------------------------------------------

<a name="q2500"></a>
### Will we work with both UDP and TCP on this project or just TCP?

Just TCP.

Although there's nothing stopping you from sending HTTP with UDP (heck, there's
nothing stopping you from writing out your HTTP data by hand and snail-mailing
it to its destination), TCP provides the reliable transport that HTTP needs, so
people use TCP. That is, they use HTTP/TCP/IP as opposed to HTTP/UDP/IP.

See also: [RFC 1149](https://tools.ietf.org/html/rfc1149).

------------------------------------------------------------------------

<a name="q2600"></a>
### What C libraries do we need for working with sockets?

On a Unix system, you don't need to specify anything additional for the build.
But you do need to include the proper header files for particular functions
you're using.

If you want to use the `socket()` syscall, for example, check out the [man page
for `socket()`](http://man7.org/linux/man-pages/man2/socket.2.html) and it'll
tell you what files you need to `#include`.

See also: [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/).

------------------------------------------------------------------------

<a name="q2700"></a>
### For the LRU cache, why do we need a doubly-linked list? Are there other data structures that could work?

One of the things the LRU cache needs to do is delete an item from the middle of
the list and move it to the head of the list. And we want to do this fast.

| Data Structure     | Delete | Add to Head |
|--------------------|:------:|:-----------:|
| Linked List        |  O(n)  |     O(1)    |
| Doubly-Linked List |  O(1)  |     O(1)    |

The doubly-linked list has the scaling characteristics that we're looking for.

If you have another data structure that works as well, you could use that
instead.

------------------------------------------------------------------------

<a name="q2800"></a>
### How do we add an age to the cache entries to remove them when they get old?

Add a timestamp to the cache entry structure.

> The `time()` library call might be good for this. It returns the current time
> in number of seconds since January 1, 1970.

Then the question is how to expire elements from the cache?

One thing that comes to mind is to periodically scan the cache for expired
entries, but this is an O(n) process.

Another thing you can do is only check for expired cache entries on a _cache
hit_. When you fetch an item from the cache and are about to serve it, check the
time. If it's too old, delete it from the cache and refresh it from disk.

------------------------------------------------------------------------

<a name="q2900"></a>
### What about an LFU cache (Least <i>Frequently</i> Used) instead of an LRU cache?

LFU is a similar caching strategy. One place it might differ is if you have a
file that got accessed a million times in a short timeframe, and then never
again. It would persist in an LFU cache for a long time until other pages got
used more.

Also new items might be removed too quickly from the cache due to their low
counts.

It's not a _bad_ strategy--it's just not one that's well-suited for our use.

See also: [Cache Replacement Policies](https://en.wikipedia.org/wiki/Cache_replacement_policies)

------------------------------------------------------------------------

<a name="q3000"></a>
### Would using Python's `OrderedDict` data structure be better or worse than using a doubly-linked list for an LRU cache?

Turns out it would be the same!

The question is always, "What's the time complexity for doing the operations we
need to do?"

| Data Structure                | Find Entry | Delete from List | Insert at Head |
|-------------------------------|:----------:|:----------------:|:--------------:|
| Hash Table/Doubly-Linked List |    O(1)    |        O(1)      |      O(1)      |
| `OrderedDict`                 |    O(1)    |        ???       |      O(1)?     |

A bit of research shows that, yes, it's O(1) to insert.

And a bit more shows that it's O(1) to delete from the `OrderedDict`--because it
is implemented using a doubly-linked list!

In fact, the `OrderedDict` implementation is already very similar to our LRU
cache in many respects.

------------------------------------------------------------------------

<a name="q3100"></a>
### I added the `Date` HTTP header using `asctime()` (or `ctime()`) but now everything shows up in my browser as plain text and some of the headers are in the body, as well.

`asctime()` and `ctime()` add a newline for you, so you don't need to add one in
your `sprintf()`.

```c
sprintf(response, "HTTP/1.1 200 OK\n"
   "Date: %s\n"   // <-- EXTRA NEWLINE
   "Connection: close\n"
```

If you do add the newline, you end up with HTTP data that looks like this:

```http
HTTP/1.1 200 OK
Date: Wed Jun 30 21:49:08 1993

Content-Type: text/html
Content-Length: 201
Connection: close

<some html body>
```

That's not what you want. The HTTP header ends at the first blank line. You want
this:

```http
HTTP/1.1 200 OK
Date: Wed Jun 30 21:49:08 1993
Content-Type: text/html
Content-Length: 201
Connection: close

<some html body>
```

and the way to get that is to take the extra newline out of the `Date` header.

```c
sprintf(response, "HTTP/1.1 200 OK\n"
   "Date: %s"   // <-- Newline removed--perfect!
   "Connection: close\n"
```

The details are buried in the [`asctime()` man
page](https://linux.die.net/man/3/asctime):

> The `asctime()` function converts the broken-down time value `tm` into a
> null-terminated string with the same format as `ctime()`.

> The call `ctime(t)` is equivalent to `asctime(localtime(t))`. It converts the
> calendar time `t` into a null-terminated string of the form
>
>     "Wed Jun 30 21:49:08 1993\n"

Note the included trailing newline.

------------------------------------------------------------------------

<a name="q3200"></a>
### Do other higher-level languages use sockets under the hood for network communication?

Short answer: yes, or something equivalent.

Sockets is actually a programming API defined for Unix and Unix-like systems
that includes the calls like `socket()`, `connect()`, `recv()`, and so on.

If the OS you're using has a sockets API implemented, then yes, the higher-level
language is using sockets. (This includes OSes like macOS, Linux, BSD, etc.)

If you're on Windows, it uses a different API called
[Winsock](https://en.wikipedia.org/wiki/Winsock), which basically does the same
thing.

------------------------------------------------------------------------

<a name="q3300"></a>
### In TCP, what is the Transmission Control Block (TCB)?

It's an internal data structure in the OS that's used to hold metadata about a
single TCP connection. It's not something that users interact with directly.
(They interact with it indirectly via the sockets programming API and the
syscalls it provides.)

TCP has a lot of work to do in order to recreate the data stream accurately. It
might be getting packets out of order, they might have errors, they might be
missing, or they might be duplicated. TCP has to manage this craziness, while
asking the remote computer for retransmissions of missing data, into a single,
correct stream to present to the user.

It needs a place to keep track of the current work for any particular
connection. That place is the TCB data structure.

* [More at Wikipedia](https://en.wikipedia.org/wiki/Transmission_Control_Protocol#Resource_usage)

------------------------------------------------------------------------

<a name="q3400"></a>
### Does UDP ever send the same data multiple times?

No. There is no automatic retransmission with UDP.

Rarely a UDP packet might get duplicated in transit by a buggy system or some
weird router configuration.

------------------------------------------------------------------------

<a name="q3500"></a>
### How much faster is UDP than TCP? How can I measure it?

The answer to this question is surprisingly complex, and there are a significant
number of cases where TCP can outperform UDP.

In general, TCP is better at managing _bandwidth_ (how much data you can shove
through the pipe in a certain amount of time), but UDP is better with _latency_
(how long it takes a packet to arrive). But there are counterexamples to this,
certainly.

A good rule of thumb is try TCP first. If it's not cutting it, then you can do
something more complex with UDP.

A tool like `iperf` can give you an indication about which type of data runs
best on your particular network configuration.

* [Informative discussion on SO](https://stackoverflow.com/questions/47903/udp-vs-tcp-how-much-faster-is-it)

------------------------------------------------------------------------

<a name="q3600"></a>
### What's a normal rate of packet loss?

It absolutely depends on tremendous variety of factors, but on a
regularly-loaded network, it's probably around 1-2%.

If you start flooding your Ethernet with tons of UDP packets as fast as you can,
you'll see a much higher loss rate.

------------------------------------------------------------------------

<a name="q3700"></a>
### Does <i>most</i> of the UDP data normally arrive?

Yes, in normal circumstances, _most_ of the UDP packets arrive. You should never
count on any of them arriving, though.

If the network is congested, or if someone puts a backhoe through a fiber trunk,
the loss rates will increase.

------------------------------------------------------------------------

<a name="q3800"></a>
### Do packets get delivered in the order they're sent or do they get jumbled up by the process?

They get jumbled. TCP unjumbles them. UDP does not.

------------------------------------------------------------------------

<a name="q3900"></a>
### Can different packets from the same request can take a different routes to stop bottle necks?

Yes, but it happens transparently to you.

You're coding at the application layer, and the routing takes place at the IP
layer, two layers down.

* [Dynamic Routing at Wikipedia](https://en.wikipedia.org/wiki/Dynamic_routing)

------------------------------------------------------------------------

<a name="q4000"></a>
### If your request hits a load balancer will it remember such that each subsequent request you send will be sent to the same server?

Yes. Otherwise writing server software would be quite a juggling act.

------------------------------------------------------------------------

<a name="q4100"></a>
### I'm trying to test with `curl` or the browser, and it's telling me "`Connection refused`". Why?

That's the error you get if you try to connect to a port and no one is listening
on it.

Typically means that your server isn't running. Start it up and then try again.

Barring that, make sure you've specified the right port number to `curl` or the
browser.

------------------------------------------------------------------------

<a name="q4200"></a>
### My server seems to hang indefinitely on the line `newfd = accept(listenfd, ...)` in the `main` function. It never moves past it when requests are made to it. What's going on?

First thing to check would be your task manager (i.e. Task Manager in Windows, or by typing `jobs` in your terminal if you have `bash`). If you look there and see that you have any server processes in a "Suspended" state, then most likely it means you're halting each server process using `CTRL-Z` instead of what you should be using to exit each server process which is `CTRL-C`. `CTRL-Z` suspends the given process, which in this case means the suspended server process is now just squatting on the port, such that that port can now no longer be accessed by a fresh server process instance if you decide to fire the server up again. 

You can move any of the suspended server processes to the foreground by typing `fg %x` where `x` is the job number of the suspended server, or kill any of them with `kill %x` where `x` is again the job number of the process.

-----------------------------------------------------------------------

<a name="q4300"></a>
### When I request the `cat.jpg` image, I just get a black screen rendered in my browser. What's the problem?

If you open up the developer console in the browser, you'll likely see `ERR CONTENT LENGTH MISMATCH`. If this is the case, the reason is that the client isn't receiving all of the data from the server. 

This is most likely happening because the response is being built using `sprintf`. `sprintf` is fine to use when the server sends text data (plaintext, html, json, etc.), which is all UTF-8 encoded. However, image data is not UTF-8 encoded. It uses a different encoding that `sprintf` cannot handle. 

What's happening is that `sprintf` is treating the image data as if it were UTF-8 encoded, and it probably encounters bytes in the image data that in UTF-8 represent a null terminator. So `sprintf` thinks that's the end of the image and doesn't write the rest of the image data to the response buffer before sending the response data to the client, so the client doesn't get all of the expected data. 

The solution is that we need to write the image data to the response buffer in a way that is encoding-agnostic. Something like `memcpy` would work very well for this, as `memcpy` doesn't assume anything about the encoding of the data. 

-----------------------------------------------------------------------

<a name="q4400"></a>
### What's the difference between `sprintf` and `snprintf`? When do I use one over the other?

The only difference between `sprintf` and `snprintf` is that `snprintf` accepts an `n` argument that specifies the maximum number of bytes that will be written to the buffer that it is writing to. The purpose of this is to better ensure that the buffer being written to doesn't overflow from too many bytes. 

As far as when to use one over the other, really, you could use either, most of the time. The only time you might want to consider `sprintf` over `snprintf` is when we don't know how many bytes we'll want written to the buffer. `sprintf` returns the number of bytes written to the buffer. 

-----------------------------------------------------------------------

<a name="q4500"></a>
### When serving a file that is in the cache, the browser renders some garbage text instead of/in addition to what is expected to be rendered.

The fact that garbage is being rendered seems to indicate that the content of the file you're sending down to the client is not well-formed. One likely cause of this could be if you called `free` on the `file_data` struct on the pointer pointing to the `file_data` struct somewhere else in your server code. This points to the likely fact that cache entries do not own the content they're responsible for. To put this more concretely, if each allocated entry only has a pointer to its content instead of a _copy_ of its content, then this is likely the cause of the problem:
```c
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    struct cache_entry *ce = malloc(sizeof *ce);
    ce->content = content;     // <--- Here, the entry only has a shared pointer to the content
    ...
}
```
With the above code, since the entry only has a _shared_ reference to to the content, the content can be `free`d from another shared pointer somewhere else in the code.

To circumvent this issue, when allocating a new entry in the cache, each cache entry should own a copy of the content that it is storing. In other words, allocate and copy additional memory for each entry that has enough space to hold a fresh copy of the content so that `free`ing the `file_data` struct somewhere else in the server doesn't affect `file_data` structs.
```c
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    struct cache_entry *ce = malloc(sizeof *ce);
    ce->content = malloc(content_length);
    memcpy(ce->content, content, content_length);  // <--- Now, the entry has its own allocated chunk of memory with the content
    ...
}
```
