#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"

/**
 * Allocate a cache entry
 */
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    // need to allocate memory for all strings
    struct cache_entry *newCache_Entry = malloc(sizeof *newCache_Entry);
    newCache_Entry->path = malloc(strlen(path) + 1);
    newCache_Entry->content_type = malloc(strlen(content_type));
    newCache_Entry->content = malloc(content);
    newCache_Entry->content_length = content_length;


    strcpy(newCache_Entry->path, content_type);
    strcpy(newCache_Entry->content_type, content_type);
    newCache_Entry->content_length = content_length;
    memcpy(newCache_Entry->content, content, content_length);
    // we have minor memory issues up here, but we can fix that later

    return newCache_Entry;
    
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

    struct cache *newCache = malloc(sizeof *newCache);
    newCache->max_size = max_size;
    newCache->cur_size = NULL;
    newCache->index  = hashtable_create(hashsize, NULL);
    newCache->head = NULL;
    newCache->tail = NULL;

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
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
void cache_put(struct cache *cache, char *path, char *content_type, void *content, int content_length)
{
// struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)


    struct cache_entry *new_cache_entry = alloc_entry(path, content_type, content, content_length); 

    dllist_insert_head(cache, new_cache_entry); 
    hashtable_put(cache->index, path, new_cache_entry);

// 3. Add a pointer to the cache entry to the hash table, indexed by the key.
    cache->cur_size++;
    while(cache->cur_size > cache->max_size) {
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
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////    
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    struct cache *getcache = hashtable_get(cache->index, path); 
    if(getcache == NULL) { 
        return NULL;
    }
    dllist_move_to_head(cache, getcache); 
    return getcache;
}
