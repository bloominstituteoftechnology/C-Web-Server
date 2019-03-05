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

<!--
TODO:

-->

<!-- ============================================================================= -->

<!--
Template:

<p><details><summary><b></b></summary><p>
</p></details></p>
-->
