# MIME types

_Multipurpose Internet Mail Extensions_ (MIME) was originally created for email,
as its name suggests.

It used to be, back in the day when cavemen were painting their hands on cave
walls, that all email was ASCII text and that was all. If you wanted to include
an image, you had to draw it in ASCII-art.

So in order to pass other kinds of data in email, MIME was invented. It allowed
emails to contain multiple parts, and had a way of identifying the type of data
that was contained within each part.

MIME is still used for email to this day.

But the designers of the web recognized that many of the same problems could be
solved by using MIME with the web, as well.

One of the problems is identifying the type of data that is arriving from the
server.

## What type is my data?

Bytes are just numbers. So when the web browser receives a stream of bytes from
the server, how can it tell if this is unicode HTML data, binary animated-gif
data, or a JPEG vacation photograph? They're all just streams of numbers!

The server has to tell the browser what type this data is. And it does this by
specifying the MIME type.

Here are some common MIME types:

* JPEG: `image/jpeg`
* GIF: `image/gif`
* PNG: `image/png`
* JavaScript: `application/javascript`
* JSON: `application/json`
* CSS: `text/css`
* HTML: `text/html`
* Plain TXT: `text/plain`
* Non-descript data: `application/octet-stream`

> _Octet_ is another name for an 8-bit value, commonly known as a _byte_.
> (Historically the number of bits in a byte varied, even though today they're
> vitually always 8 bits. An octet is _always_ 8 bits by definition.)

There are [a lot of MIME
types](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Complete_list_of_MIME_types).

## How to determine the type of data served?

### Programmatic endpoints

If you have a programmatic endpoint (e.g. an endpoint that generates the data
instead of reading it from disk) then you simply specify the type of the data
you're sending back.

For example, if you return data is:

```json
{
    "animal_type": "goat",
    "count": 37
}
```

then you'll use the content type of `application/json`.

### File serving

But what if you're reading data from a file and serving it?

If the client requests `http://example.com/foo.png`, we need to reply with a
type of `image/png`. The usual way to do this is to simply map between the file
extension `.png` and its MIME type `image/png`.

1. Isolate the file extension.

   Examples:
   
   File is `frotz.jpg`, extension is `.jpg`.

   File is `foo.bar.txt`, extension is `.txt`.

2. Map the extension to its MIME type.

   Example: `.txt` maps to `text/plain`

3. If you can't find a mapping for an extension, use `application/octet-stream`.

Comparisons are case-insensitive. Here are some examples:

* `.jpg` or `.jpeg`: `image/jpeg`
* `.gif`: `image/gif`
* `.png`: `image/png`
* `.js`: `application/javascript`
* `.json`: `application/json`
* `.css`: `text/css`
* `.htm` or `.html`: `text/html`
* `.txt`: `text/plain`
* `.ico`: `image/x-icon`

## How is the MIME type returned?

It comes back in the HTTP header in the `Content-Type` field:

```
Content-Type: text/html
```

Whichever library you are using should have a way to set the `Content-Type`
header.

Webservers construct their own HTTP headers so the `Content-Type` is included at
that time.

## What does a browser (or other client) do with the MIME type?

The browser uses the MIME type of the data to decide what to do with it. If it's
`text/html`, it displays it. Probably the same with `image/*` data. Also
`text/plain`.

The browser can display a number of types of data natively. But what if it gets
one it doesn't know? There are, after all, about 1.2 zillion MIME types.

In that case, it has two options:

1. It can spawn an external viewer to show the file, if it is aware of one.

2. It can simply download the file and let the user deal with it later.