# Least-Recently Used Cache (LRU Cache)

## Caches

A cache is a place that stores copies of commonly-used information for quick
retrieval and lookup. Hitting the cache for a piece of data should be faster
than getting it from the original source.

The idea is that if lots of people want access to this piece of data, you copy
it into the faster cache so they all get it in a more timely manner.

There is a cache on your CPU that keeps commonly-used parts of RAM so the CPU
doesn't have to wait for the slower RAM each time.

Your web browser keeps a cache of web pages and other web data so that it
doesn't have to retrieve it over the network every time you go to a page,
greatly speeding up page load times.

A web server can also cache files so that it doesn't have to go through the
operating systems to get files off disk every time. It can just retrieve the
file from RAM and serve it directly.

## LRU Caches

Of course, you can't cache _everything_ _forever_. You'd need a cache as big as
all the stuff you're wanting to store, if you wanted to do that. And a web
server might have access to several terabytes of data, but only have a few
gigabytes of RAM.

So you have to be selective. It makes a lot of sense to have a cache of limited
size, and fill that cache with the most frequently-accessed data. 

One strategy for doing this is called an _LRU cache_. This cache type has a
fixed number of entries (or size in bytes).

In the context of a webserver, when a file is requested by a client, the
following steps occur:

1. Check if the file is in the cache.

2. If it is, serve it. We're done.

3. If it's not in the cache, load it from disk.

4. Save it in the cache.

5. Serve it.

If the cache exceeds its max size, the _least recently used_ entry in the cache
is discarded.

## How to build an LRU cache

### Structure

LRU caches are made of two cooperating data structures: a doubly linked list and
a hash table.

* The hash table is used for quick lookup into the doubly linked list.

* The doubly linked list is used to keep track of which entries are
  least-recently used.
  
  The item at the head of the list is the most-recently used.
  
  The item at the tail is the least-recently used.

### Putting items in the cache

The item will be identified by a key (in the case of a webserver, the key should
be a string containing the path on disk to the file in question).

A _cache entry_ will contain copies of all the information needed to serve the
file again if it is retrieved from the cache. For a webserver, this is things
like content MIME type, content length, and the content itself.

Additionally the cache entry should have `prev` and `next` pointers so it can be
used in a doubly linked list.

Finally, it should also contain a copy of the key.

For putting an item in the cache:

1. Construct a new cache entry containing all the required information.

2. Add a pointer to the cache entry to the head of the doubly linked list.

3. Add a pointer to the cache entry to the hash table, indexed by the key.

4. If the cache is larger than its max allowable size, see [LRU
   discarding](#lru-discarding), below.

### Getting items from the cache

1. Look up the cache entry pointer in the hash table with the key.

2. If it's not in the hash table, it's not in the cache. We're done.

3. If it is found, the cache entry is cut out of the doubly linked list.

4. The cache entry is inserted at the head of the list. (Because it's now the
   most-recently used.)

Since every time you get an item from the cache it moves to the head of the
list, it becomes most-recently used. In this way, frequently-used entries tend
to bubble up to the head of the list and less-frequently-used entries tend to
fall to the tail of the list.

### LRU discarding

If the cache is larger than its allowable max size, the least-recently used
items must be discarded.

This typically happens when an item is inserted in the list that pushes it oversize.

* while size > max_size:
  * get the cache entry at the tail of the list
  * remove it from the tail of the list
  * using its key, remove it from the hash table