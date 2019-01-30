#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"

/**
 * Allocate a cache entry
 */
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    // 1. Create cache_entry and initialize
    struct cache_entry *cache_entry_struct = malloc(sizeof(struct cache_entry));
    cache_entry_struct->path = path;
    cache_entry_struct->content_type = content_type;
    cache_entry_struct->content_length = content_length;
    cache_entry_struct->content = content;

    return cache_entry_struct;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
}

/**
 * Insert a cache entry at the head of the linked list
 */
void dllist_insert_head(struct cache *cache, struct cache_entry *ce)
{
    // Insert at the head of the list
    if (cache->head == NULL)
    {
        cache->head = cache->tail = ce;
        ce->prev = ce->next = NULL;
    }
    else
    {
        cache->head->prev = ce;
        ce->next = cache->head;
        ce->prev = NULL;
        cache->head = ce;
    }
}

/**
 * Move a cache entry to the head of the list
 */
void dllist_move_to_head(struct cache *cache, struct cache_entry *ce)
{
    if (ce != cache->head)
    {
        if (ce == cache->tail)
        {
            // We're the tail
            cache->tail = ce->prev;
            cache->tail->next = NULL;
        }
        else
        {
            // We're neither the head nor the tail
            ce->prev->next = ce->next;
            ce->next->prev = ce->prev;
        }

        ce->next = cache->head;
        cache->head->prev = ce;
        ce->prev = NULL;
        cache->head = ce;
    }
}

/**
 * Removes the tail from the list and returns it
 * 
 * NOTE: does not deallocate the tail
 */
struct cache_entry *dllist_remove_tail(struct cache *cache)
{
    struct cache_entry *oldtail = cache->tail;

    cache->tail = oldtail->prev;
    cache->tail->next = NULL;

    cache->cur_size--;

    return oldtail;
}

/**
 * Create a new cache
 * 
 * max_size: maximum number of entries in the cache
 * hashsize: hashtable size (0 for default)
 */
struct cache *cache_create(int max_size, int hashsize)
{
    // 1. Create a cache struct (malloc sizeof cache)
    struct cache *cache_struct = malloc(sizeof *cache_struct);

    // 2. Initialize cache's attributes
    cache_struct->max_size = max_size;
    cache_struct->cur_size = 0;
    cache_struct->index = hashtable_create(hashsize, NULL);
    cache_struct->head = NULL;
    cache_struct->tail = NULL;

    // 3. return the struct
    return cache_struct;
}

void cache_free(struct cache *cache)
{
    struct cache_entry *cur_entry = cache->head;

    hashtable_destroy(cache->index);

    while (cur_entry != NULL)
    {
        struct cache_entry *next_entry = cur_entry->next;

        free_entry(cur_entry);

        cur_entry = next_entry;
    }

    free(cache);
}

/**
 * Store an entry in the cache
 *
 * This will also remove the least-recently-used items as necessary.
 * 
 * NOTE: doesn't check for duplicate cache entries
 */

/*
- Allocate a new cache entry with the passed parameters.
- Insert the entry at the head of the doubly-linked list.
- Store the entry in the hashtable as well, indexed by the entry's path.
- Increment the current size of the cache.
-   If the cache size is greater than the max size:
-       Remove the entry from the hashtable, using the entry's path and the hashtable_delete function.
-       Remove the cache entry at the tail of the linked list (this is the least-recently used one)
-       Free the cache entry.
-       Ensure the size counter for the number of entries in the cache is correct.
 */
void cache_put(struct cache *cache, char *path, char *content_type, void *content, int content_length)
{
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
}