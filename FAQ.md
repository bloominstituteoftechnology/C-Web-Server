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


<!--
TODO:

-->

<!-- ============================================================================= -->

<!--
Template:

<p><details><summary><b></b></summary><p>
</p></details></p>
-->
