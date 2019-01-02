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
   //Allocate some memory
    struct cache_entry* cache_entry_inst = malloc(sizeof(struct cache_entry));

    //Create obj props for the data structure
    cache_entry_inst->path = path;
    cache_entry_inst->content_type = content_type;
    cache_entry_inst->content_length = content_length;
    cache_entry_inst->content = content;

    // return the list
    return cache_entry_inst;

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
     //Allocate some mem
    struct cache *cache_inst = malloc(sizeof(struct cache));
    struct hashtable *ht = hashtable_create(hashsize, NULL);

    //Create obj props for the data structure
    cache_inst->head = NULL;
    cache_inst->tail = NULL;
    cache_inst->cur_size = 0;
    cache_inst->max_size = max_size;
    cache_inst->index = ht;

    //Return the data structure
    return cache_inst;
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
    // Allocate for new cache entry with passed params
    struct cache_entry *allocated_entry = alloc_entry(path, content_type, content, content_length);

    // Insert entry at head of double linked list
    dllist_insert_head(cache, allocated_entry);

    // Store entry in hash table (indexed by the entrys path)
    hashtable_put(cache->index, path, allocated_entry);

    // Increment current size of cache
    printf("Size before inc: %d\n", cache->cur_size);
    cache->cur_size++;
    printf("Size after inc: %d\n", cache->cur_size);

    // If cache size is greater than max size
    if (cache->cur_size > cache->max_size){
        printf("Went over the max\n");

        // Remove entry from hash table from entrys path and hashtable_delete
        hashtable_delete(cache->index, cache->tail->path);

        // Remove cache entry at tail of linked list
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
}
