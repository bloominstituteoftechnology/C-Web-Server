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
  struct cache_entry* cache_entry_inst = malloc(sizeof(struct cache_entry));

  //Define/init the attribtes or properties of the obj/DS
  cache_entry_inst->path = path;
  cache_entry_inst->content_type = content_type;
  cache_entry_inst->content_length = content_length;
  cache_entry_inst->content = content;

  //return the DS
return cache_entry_inst;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
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
  struct cache_entry *oldtail = cache->tail;

  cache->tail = oldtail->prev;
  cache->tail->next = NULL;

  cache->cur_size--;

return oldtail;
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
    // Allocate a new cache entry with passed parameters
    struct cache_entry *allocated_entry = alloc_entry(path,
    content_type, content, content_length);

    // Insert an entry at the head of the doubly-linked list
    dllist_insert_head(cache, allocated_entry);

   // Store entry in hashtable , indexed by entry's path
   hashtable_put(cache->index, path, allocated_entry);

   // Increment the size of the cache
   printf("Size before inc: %d\n", cache->cur_size);
   cache->cur_size++;
   printf("Size after inc: %d\n", cache->cur_size);

   // If cache size is greater than the max size:
   if (cache->cur_size > cache->max_size) {
       printf("Over max\n");

       // remove entry from hashtable
       hashtable_delete(cache->index, cache->tail->path);

       // remove the tail and set it to a variable
       struct cache_entry *old_tail = dllist_remove_tail(cache);
       cache->tail->prev = old_tail->prev->prev;

        // Remove the cache entry at the tail of the linked list.
        free_entry(old_tail);
   }

}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    struct cache_entry *entry = hashtable_get(cache->index,
    path);

    if(entry != NULL) {
        dllist_move_to_head(cache,entry);
        return entry;
    }
    return NULL;
}
