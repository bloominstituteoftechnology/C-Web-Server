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
    struct cache_entry *ce = malloc(sizeof *ce);
    ce->path = malloc(strlen(path) + 1);
    strcpy(ce->path, path);
    ce->content_type = malloc(strlen(content_type) + 1);
    strcpy(ce->content_type, content_type);
    ce->content = malloc(content_length);
    memcpy(ce->content, content, content_length);
    ce->content_length = content_length;
    return ce;
}
/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *v_ent)
{
    // free the data? Yup!
    
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
    struct cache *cache = malloc(sizeof *cache);
    // reference to DLL
    cache->head = NULL;
    cache->tail = NULL;
    // create hash reference, we dont know what 2nd parameter is, so we just gave it NULL
    cache->index = hashtable_create(hashsize, NULL);
    // max size in struct = size given
    cache->max_size = max_size;
    cache->cur_size = 0;
    return cache;
}
void cache_free(struct cache *cache)
{
    struct cache_entry *curr_entry = cache->head;
    while (curr_entry != NULL)
    {
        struct cache_entry *ne = curr_entry->next;
        free_entry(curr_entry);
        curr_entry = ne;
    }
    hashtable_destroy(cache->index);
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
    // make cache entry
    struct cache_entry *ce = alloc_entry(path, content_type, content, content_length);
    // add to head of DLL
    dllist_insert_head(cache, ce);
    // put in hashtable
    hashtable_put(cache->index, path, ce);
    cache->cur_size++;
}
/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    // check if ce exists in hash
    // using cache entry so create a struct?
    struct cache_entry *ce;
    ce = hashtable_get(cache->index, path);
    // if cache entry == NULL, print error
    if (ce == NULL)
    {
        return NULL;
    }
    // else return data, and move to front of DLL to keep from getting removed
    dllist_move_to_head(cache, ce);
    return ce;
}