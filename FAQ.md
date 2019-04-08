# Web Server FAQ

<!-- ============================================================================= -->

<p><details><summary><b>Why do we have to specify the <tt>Host</tt> field in the HTTP request? Isn't it redundant since we just connected to that host?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>In <tt>send_response()</tt>, the <tt>body</tt> argument is passed as a <tt>void*</tt>. Is there some scenario where the <tt>body</tt> is not a string?</b></summary><p>

In the MVP, no it's always a string.

But if you want to serve any kind of binary file (e.g. `foo.jpg`), you'll be
`send()`ing data that's _not_ a string.

This is why `send_response()` takes a `void*`--it can point to any type of data,
string or not.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>I rebuilt and re-ran the server, but nothing's happening.</b></summary><p>

You have to hit it with a web request. Either send your browser to
`http://localhost:3490/`, or run curl:

```shell
curl -D - http://localhost:3490/
```

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Why is the <tt>Content-Length</tt> different than the number of bytes we pass to <tt>send()</tt>?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Does the order of the fields in the HTTP header matter?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>After sending the request and getting the response, my client is just sitting there, waiting, and not closing the connection.</b></summary><p>

Make sure you have the

```http
Connection: close
```

field in your header.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>After sending a <tt>GET</tt> request, the server is just sitting there waiting for something and not responding.</b></summary><p>

Make sure you end your request header with a blank line. That's how the server
knows the header is complete.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How can I use <tt>curl</tt> on the command line to test my server?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How do browsers handle caches?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What is MIME? Outside of web servers, where are MIME types used?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Where can I find a complete list of MIME types?</b></summary><p>

Since people are adding new MIME types all the time, there's not really a such
thing as a _complete_ list.

(You can make up your own MIME types. If they're not official, the second part
of the MIME type should be prefixed with `x-`, like `application/x-bzip`.)

Common MIME types can be found at [MDN's incomplete list of MIME
types](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Complete_list_of_MIME_types).

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How does MIME multipart work?</b></summary><p>

It splits the body up into separate MIME sections, each with its own MIME type.

See Wikipedia: [MIME multipart
messages](https://en.wikipedia.org/wiki/MIME#Multipart_messages)

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Our server only sends out one file at a time. How do browsers get multiple files, like if <tt>index.html</tt> refers to <tt>styles.css</tt>?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How can I fix my server to return binary data as well as text data? I want to get <tt>cat.png</tt> back to the browser.</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>I tried to do the <tt>telnet</tt> demo, but it instantly comes back with a 501 response.</b></summary><p>

Telnet can optionally send some control commands back and forth per the [telnet
protocol](https://tools.ietf.org/html/rfc854) and it seems the Windows version
of telnet does this. Try the
[WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) version.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What's the main difference between TCP and UDP, and when to use each?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Does the "T" in TCP stand for Transport or Transmission?</b></summary><p>

It stands for [Transmission](https://tools.ietf.org/html/rfc793).

Confusingly, it is located at the Transport layer of the [OSI layered network
model](https://en.wikipedia.org/wiki/OSI_model).

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How are UDP packets lost?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What are the layers of the network stack?</b></summary><p>

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

</p></details></p>


<!-- ============================================================================= -->

<p><details><summary><b>How do you know which layer of the network stack is causing a problem?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What does the <tt>flags</tt> parameter do in <tt>send()</tt> and <tt>recv()</tt>?</b></summary><p>

It gives you additional control over how the data is sent. (Using `read()` or
`write()` with a socket descriptor is the same as calling `recv()` or `send()`
with the `flags` set to `0`.)

Practically speaking, the only flag that is remotely commonly used is `MSG_OOB`,
which is used to send _out of band_ data to the receiver. This is data that can
be logically thought of as being being outside the normal data stream, e.g. for
priority or exception data. The receiving side gets a signal indicating that
some out of band data has arrived and that normal processing should be
interrupted to handle it.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What is that <tt>fd</tt> variable?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What is an "octet"?</b></summary><p>

It's another word for _byte_, practically speaking.

Specifically, an octet is a number that is exactly 8 bits long.

Although a byte is also 8 bits long on basically all modern architectures,
historically there have been systems where a byte was a different number of
bits.

The term _octet_ removes the ambiguity.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Will we be diving into HTTPS or any type of secured connection protocols?</b></summary><p>

Not in this class.

HTTPS runs HTTP on top of another protocol called
[TLS](https://en.wikipedia.org/wiki/Transport_Layer_Security). There are [a
number of TLS implementations you can choose
from](https://en.wikipedia.org/wiki/Comparison_of_TLS_implementations).

HTTP works the same in both HTTP and HTTPS; with the latter, the TLS layer is
responsible for the encryption.

But the details of how that is done is beyond the scope of the class.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Will we work with both UDP and TCP on this project or just TCP?</b></summary><p>

Just TCP.

Although there's nothing stopping you from sending HTTP with UDP (heck, there's
nothing stopping you from writing out your HTTP data by hand and snail-mailing
it to its destination), TCP provides the reliable transport that HTTP needs, so
people use TCP. That is, they use HTTP/TCP/IP as opposed to HTTP/UDP/IP.

See also: [RFC 1149](https://tools.ietf.org/html/rfc1149).

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What C libraries do we need for working with sockets?</b></summary><p>

On a Unix system, you don't need to specify anything additional for the build.
But you do need to include the proper header files for particular functions
you're using.

If you want to use the `socket()` syscall, for example, check out the [man page
for `socket()`](http://man7.org/linux/man-pages/man2/socket.2.html) and it'll
tell you what files you need to `#include`.

See also: [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/).

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>For the LRU cache, why do we need a doubly-linked list? Are there other data structures that could work?</b></summary><p>

One of the things the LRU cache needs to do is delete an item from the middle of
the list and move it to the head of the list. And we want to do this fast.

| Data Structure     | Delete | Add to Head |
|--------------------|:------:|:-----------:|
| Linked List        |  O(n)  |     O(1)    |
| Doubly-Linked List |  O(1)  |     O(1)    |

The doubly-linked list has the scaling characteristics that we're looking for.

If you have another data structure that works as well, you could use that
instead.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How do we add an age to the cache entries to remove them when they get old?</b></summary><p>

Add a timestamp to the cache entry structure.

> The `time()` library call might be good for this. It returns the current time
> in number of seconds since January 1, 1970.

Then the question is how to expire elements from the cache?

One thing that comes to mind is to periodically scan the cache for expired
entries, but this is an O(n) process.

Another thing you can do is only check for expired cache entries on a _cache
hit_. When you fetch an item from the cache and are about to serve it, check the
time. If it's too old, delete it from the cache and refresh it from disk.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What about an LFU cache (Least <i>Frequently</i> Used) instead of an LRU cache?</b></summary><p>

LFU is a similar caching strategy. One place it might differ is if you have a
file that got accessed a million times in a short timeframe, and then never
again. It would persist in an LFU cache for a long time until other pages got
used more.

Also new items might be removed too quickly from the cache due to their low
counts.

It's not a _bad_ strategy--it's just not one that's well-suited for our use.

See also: [Cache Replacement Policies](https://en.wikipedia.org/wiki/Cache_replacement_policies)

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Would using Python's <tt>OrderedDict</tt> data structure be better or worse than using a doubly-linked list for an LRU cache?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>I added the <tt>Date</tt> HTTP header using <tt>asctime()</tt> (or <tt>ctime()</tt>) but now everything shows up in my browser as plain text and some of the headers are in the body, as well.</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Do other higher-level languages use sockets under the hood for network communication?</b></summary><p>

Short answer: yes, or something equivalent.

Sockets is actually a programming API defined for Unix and Unix-like systems
that includes the calls like `socket()`, `connect()`, `recv()`, and so on.

If the OS you're using has a sockets API implemented, then yes, the higher-level
language is using sockets. (This includes OSes like macOS, Linux, BSD, etc.)

If you're on Windows, it uses a different API called
[Winsock](https://en.wikipedia.org/wiki/Winsock), which basically does the same
thing.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>In TCP, what is the Transmission Control Block (TCB)?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Does UDP ever send the same data multiple times?</b></summary><p>

No. There is no automatic retransmission with UDP.

Rarely a UDP packet might get duplicated in transit by a buggy system or some
weird router configuration.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How much faster is UDP than TCP? How can I measure it?</b></summary><p>

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

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What's a normal rate of packet loss?</b></summary><p>

It absolutely depends on tremendous variety of factors, but on a
regularly-loaded network, it's probably around 1-2%.

If you start flooding your Ethernet with tons of UDP packets as fast as you can,
you'll see a much higher loss rate.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Does <i>most</i> of the UDP data normally arrive?</b></summary><p>

Yes, in normal circumstances, _most_ of the UDP packets arrive. You should never
count on any of them arriving, though.

If the network is congested, or if someone puts a backhoe through a fiber trunk,
the loss rates will increase.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Do packets get delivered in the order they're sent or do they get jumbled up by the process?</b></summary><p>

They get jumbled. TCP unjumbles them. UDP does not.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Can different packets from the same request can take a different routes to stop bottle necks?</b></summary><p>

Yes, but it happens transparently to you.

You're coding at the application layer, and the routing takes place at the IP
layer, two layers down.

* [Dynamic Routing at Wikipedia](https://en.wikipedia.org/wiki/Dynamic_routing)

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>if your request hits a load balancer will it remember such that each subsequent request you send will be sent to the same server?</b></summary><p>

Yes. Otherwise writing server software would be quite a juggling act.

</p></details></p>

<!--
TODO:
-->

<!-- ============================================================================= -->

<!--
Template:

<p><details><summary><b></b></summary><p>
</p></details></p>

-->
