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
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    struct cache_entry *entry = malloc(sizeof(struct cache_entry));
    entry->path = path;
    entry->content_type = content_type;
    entry->content_length = content_length;
    entry->content = content;
    return entry;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    free(entry);
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
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    // allocate memory for cache structure
    struct cache *cache = malloc(sizeof(struct cache));
    // create hashtable
    struct hashtable *hashtable = hashtable_create(hashsize, NULL);
    // cache index equal to newly created hashtable
    cache->index = hashtable;
    // initialized to NULL
    cache->head = NULL;
    // initialized to NULL
    cache->tail = NULL;
    // max cache size
    cache->max_size = max_size;
    // initialized to 0
    cache->cur_size = 0;
    // return cache struct
    return cache;
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
void cache_put(struct cache *cache, char *path, char *content_type, void *content, int content_length)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // create new entry
    struct cache_entry *entry = alloc_entry(path, content_type, content, content_length);
    // add new entry to the front of the cache
    dllist_insert_head(cache, entry);
    // add entry in hashtable
    hashtable_put(cache->index, entry->path, entry);
    // incerase current size current cache size counter
    cache->cur_size++;
    // determine if cache is larger than allowed
    if (cache->cur_size > cache->max_size)
    {
        // delete hashtable by key
        hashtable_delete(cache->index, cache->tail->path);
        // delete and free cache tail 
        free_entry(dllist_remove_tail(cache));
    }
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // find entry pointer in hashtable
    struct cache_entry *entry = hashtable_get(cache->index, path);
    // check to see if entry exists in hashtable, if no return null, otherwise move entry to head of the list and return entry
    if (entry == NULL)
    {
       return NULL;
    }else{
        dllist_move_to_head(cache, entry);
        return entry;
    }
    
}
