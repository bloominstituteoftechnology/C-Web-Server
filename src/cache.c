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
    struct cache_entry *ce = malloc(sizeof(*ce));

    ce->path = malloc(strlen(path));
    strcpy(ce->path, path);

    ce->content_type = malloc(strlen(content_type));
    strcpy(ce->content_type, content_type);

    ce->content_length = content_length;

    ce->content = malloc(content_length);
    memcpy(ce->content, content, content_length);

    return ce;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *v_ent)
{
    free(v_ent->content);
    free(v_ent->content_type);
    free(v_ent->path);
    free(v_ent);

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
    struct cache *c = malloc(sizeof *c);

    c->index = hashtable_create(max_size, NULL);
    c->head = NULL;
    c->tail = NULL;
    c->max_size = max_size;
    c->cur_size = 0;

    return c;
}

/**
 * Deallocate an entire cache
**/
void cache_free(struct cache *c)
{
    struct cache_entry *curr = c->head;
    while(curr != NULL)
    {
        struct cache_entry *next = curr->next;
        free_entry(curr);
        curr = next;
    }
    hashtable_destroy(c->index);
    free(c);
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
    // make a new cache entry
    struct cache_entry *ce = alloc_entry(path, content_type, content, content_length);
    // if cache is full, remove tail from dll and hashtable
    if (cache->cur_size == cache->max_size)
    {
        hashtable_delete(cache->index, path);
        dllist_remove_tail(cache);
        cache->cur_size--;
    }
    //put into hastable
    hashtable_put(cache->index, path, ce);
    // add to head of linked list
    dllist_insert_head(cache, ce);
    // insert into hash table

    cache->cur_size++;
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    // look for entry in hashtable using path
    struct cache_entry *ce = hashtable_get(cache->index, path);

    // if not there, return NULL
    if (ce == NULL)
    {
        return NULL;
    }
    // if there 1)move to head of dll, 2)return ce
    dllist_move_to_head(cache, ce);
    return ce;
}
