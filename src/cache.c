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

    struct cache_entry *entry = malloc(sizeof(struct cache_entry));

    entry->path = strdup(path);
    entry->content_type = strdup(content_type);
    entry->content_length = content_length;
    entry->content = strdup(content);

    entry->prev = entry->next = NULL;

    return entry;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
 
    // Free all dynamically allocated memory (see alloc_entry above)
    free(entry->content);
    free(entry->content_type);
    free(entry->path);
    free(entry);
}

/**
 *  Deallocates a cache 
 */
void cache_free(struct cache *cache)
{
    struct cache_entry *cur_entry = cache->head;

    hashtable_destroy(cache->index);

    while (cur_entry != NULL) {
        struct cache_entry *next_entry = cur_entry->next;

        free_entry(cur_entry);

        cur_entry = next_entry;
    }
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

    struct cache *new_cache = malloc(sizeof(struct cache));
    new_cache->index = hashtable_create(hashsize, NULL);
    new_cache->head = new_cache->tail = NULL;
    new_cache->max_size = max_size;
    new_cache->cur_size = 0;

    return new_cache;

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
    cache->cur_size++;

    if(cache->cur_size > cache->max_size)  // If we've reached cache capacity
    {

        // remove the tail from the linked list and hashtable
        struct cache_entry *temp = dllist_remove_tail(cache);

        hashtable_delete(cache->index, temp->path);

        free_entry(temp);

    }

    // create entry and add to hash table
    struct cache_entry *entry = alloc_entry(path, content_type, content, content_length);
    hashtable_put(cache->index, entry->path, entry);
    

    // Insert entry into head of linked list
    dllist_insert_head(cache, entry);


}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{   

    struct cache_entry *res = hashtable_get(cache->index, path);

    if(res != NULL)
    {
        dllist_move_to_head(cache, res);
        return res;
    }
    
    return NULL;

}
