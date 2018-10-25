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
    struct cache_entry *ce = malloc(sizeof *ce);

    // Set the fields in the new cache entry
    ce->path = malloc(strlen(path) + 1);
    strcpy(ce->path, path);

    ce->content_type = malloc(strlen(content_type) + 1);
    strcpy(ce->content_type, content_type);

    ce->content_length = content_length;

    ce->content = malloc(content_length);
    memcpy(ce->content, content, content_length);

    return ce;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    free(entry->content_type);
    free(entry->content);
    free(entry->path);
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

// Removes the head 

struct cache_entry *dllist_remove_head(struct cache *cache)
{
    struct cache_entry *oldhead = cache->head;
    cache->head = oldhead->next;

    if (cache->head != NULL)
    {
        cache->head->prev = NULL;
    }

    cache->cur_size--;

    return oldhead;
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
    struct hashtable *hash = hashtable_create(hashsize, NULL);
    struct cache *cache_inst = malloc(sizeof(struct cache));
    cache_inst->head = NULL;
    cache_inst->tail = NULL;
    cache_inst->cur_size = 0;
    cache_inst->max_size = max_size;
    cache_inst->index = hash;
    return cache_inst;
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
    struct cache_entry *ce = alloc_entry(path, content_type, content, content_length);
    dllist_insert_head(cache, ce);
    hashtable_put(cache->index, ce->path, ce);
    cache->cur_size++;
    if (cache->cur_size > cache->max_size)
    {
        hashtable_delete(cache->index, cache->tail->path);
        free_entry(dllist_remove_tail(cache));
        if (cache->cur_size > cache->max_size)
        {
            printf("Capacity is not enough!");
        }
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
    struct cache_entry *ce;

    ce = hashtable_get(cache->index, path);

    if (ce == NULL)
    {
        return NULL;
    }
    dllist_move_to_head(cache, ce);

    return ce;
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
}

void cache_remove(struct cache *cache, struct cache_entry *entry)
{
    if (cache->head == entry && cache->tail == entry)
    {
        dllist_remove_head(cache);
        dllist_remove_tail(cache);
    }
    else if (cache->tail == entry)
    {
        dllist_remove_tail(cache);
    }
    else if (cache->head == entry)
    {
        dllist_remove_head(cache);
    }
    else
    {
        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;
    }

    hashtable_delete(cache->index, entry->path);
    free_entry(entry);
}
