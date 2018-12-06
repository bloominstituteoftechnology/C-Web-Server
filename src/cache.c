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

    struct cache_entry *newCache_Entry = malloc(sizeof(struct cache));
    newCache_Entry->path = path;
    newCache_Entry->content_type = content_type;
    newCache_Entry->content = content;
    newCache_Entry->content_length = content_length;
    // we have minor memory issues up here, but we can fix that later

    if(newCache_Entry == NULL){
        return NULL;
    }

    return newCache_Entry;
    

    // Return the pointer to the new cache_entry

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
 * max_size: maximum number of entries   in the cache
 * hashsize: hashtable size (0 for default)
 */
struct cache *cache_create(int max_size, int hashsize)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    struct cache *newCache = malloc(sizeof(struct cache));
    newCache->max_size = max_size;
    newCache->cur_size = hashsize;
    newCache->index  = hashtable_create(malloc(sizeof(struct hashtable)), NULL);
    // newCache->head = dllist_insert_head(cache, cache_entry);
    return newCache;
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
    // not sure what to do with the other members of the cache_entry struct

    int size = cache->cur_size;
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    while(size > cache->max_size) {
        dllist_remove_tail(cache);
        free_entry(cache);
    }
//   * get the cache entry at the tail of the list
//   * remove it from the tail of the list
//   * using its key, remove it from the hash table
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////


    
// 1. Look up the cache entry pointer in the hash table with the key.

    struct cache *getcache = hashtable_get(); // here, save the result of your access in a variable
// 2. If it's not in the hash table, it's not in the cache. We're done.
    if(hashtable_get() == NULL) { // check if the result is NULL
        return NULL;
    }
// 3. If it is found, the cache entry is cut out of the doubly linked list.
    else{

    }
// 4. The cache entry is inserted at the head of the list. (Because it's now the
//    most-recently used.)
    dllist_move_to_head(cache, cache_entry);
    // dllist_move_to_head both removes it from its current position in DLL
    // and makes it the new head. A lot of these prebuilt functions do a lot!
}
