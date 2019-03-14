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
    //allocate space for the struct were going to return
    struct cache_entry *ce = malloc(sizeof *ce);  //size of dereference ce if dereffed it is cache_entry
    //set all fields to values being passed in to initialize
    //have a pointer to something but have to make sure what you are pointing to persists(make sure it doesnt accidentally get freed)
    ce->path = malloc(strlen(path) + 1); //so make a copy of path we know will persist. +1 for null terminator
    strcpy(ce->path, path);  //(dest, args) copy path into allocated memory. this is deep copy.

    ce->content_type = malloc(strlen(content_type) +1);
    strcpy(ce->content_type, content_type);

    ce->content = malloc(content_length); //content is void pointer so dont know what type so cant use strlen, but know its length so can use that 
    memcpy(ce->content, content, content_length); //(dest, src, size)

    ce->content_length = content_length;  //int so dont have to malloc 

    ce->prev = ce->next = NULL;

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
    //have to free the cache entry passed in but have to free everything we malloc'ed space for
    //free everything in stucture, then free the structure itself so entry last
    free(entry->path);
    free(entry->content_type);
    free(entry->content);
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
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////  going in opposite order from cache free below. also refer to cache struct in cache.h
    //allocate cache
    struct cache *cache = malloc(sizeof *cache);
    //functions in hashtable.h and c
    cache->index = hashtable_create(hashsize, NULL); //size and pointer to a function (that takes in certain values and returns int)
    //initialize head and tail null
    cache->head = cache->tail = NULL;

    cache->max_size = max_size;

    cache->cur_size = 0;   
    
    return cache;
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
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

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
