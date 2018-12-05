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
    struct cache_entry *c_entry = malloc(sizeof(struct cache_entry));

    c_entry->path = path;
    c_entry->content_type = content_type;
    c_entry->content = content;
    c_entry->content_length = content_length;

    return c_entry;

}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    /* free(entry->path);
    free(entry->content_type);
    free(entry->content); */
    free(entry);
}

/**
 * Insert a cache entry at the head of the linked list
 */
void dllist_insert_head(struct cache *cache, struct cache_entry *ce)
{
    // Insert at the head of the list
    if (cache->head == NULL) {
        cache->head = cache->tail = ce;
        ce->prev = ce->next = NULL;
    } else {
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
    if (ce != cache->head) {
        if (ce == cache->tail) {
            // We're the tail
            cache->tail = ce->prev;
            cache->tail->next = NULL;

        } else {
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
    struct cache *ccache = malloc(sizeof(struct cache));

    ccache->index = hashtable_create(hashsize, NULL);
    ccache->head = NULL;
    ccache->tail = NULL;
    ccache->max_size = max_size;
    ccache->cur_size = 0;

    return ccache;
}

void cache_free(struct cache *cache)
{
    struct cache_entry *cur_entry = cache->head;

    hashtable_destroy(cache->index);

    while (cur_entry != NULL) {
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
    struct cache_entry *c_entry = alloc_entry(path, content_type, content, content_length);
    dllist_insert_head(cache, c_entry);
    hashtable_put(cache->index, c_entry->path, c_entry);
    cache->cur_size++;

    while (cache->cur_size > cache->max_size) {
        hashtable_delete(cache->index, cache->tail->path);
        free(dllist_remove_tail(cache));
    }
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    struct cache_entry *c_entry = hashtable_get(cache->index, path);
    if (c_entry == NULL) {
        return NULL;
    } else {
        dllist_move_to_head(cache, c_entry);
        return c_entry;
    }
}
