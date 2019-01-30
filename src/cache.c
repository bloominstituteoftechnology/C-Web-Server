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

    ce->path = path;
    ce->content_type = content_type;
    ce->content = content;
    ce->content_length = content_length;
    ce->prev = NULL;
    ce->next = NULL;

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

    if (entry != NULL) {
      free(entry);
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
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    struct cache *c = malloc(sizeof *c);

    c->index = hashtable_create(hashsize, NULL);
    c->max_size = max_size;
    c->cur_size = 0;
    c->head = NULL;
    c->tail = NULL;
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

    // Allocate a new cache entry
    struct cache_entry *new = alloc_entry(path, content_type, content, content_length);

    // Insert the entry at the head of the dll
    dllist_insert_head(cache, new);

    // Store the entry in the hashtable
    hashtable_put(cache->index, path, new);

    // Increment the current size
    cache->cur_size++;

    // Check if the size is greater than the max size
    if (cache->cur_size > cache->max_size) {

      // Remove the LRU entry from the hastable
      hashtable_delete(cache->index, cache->tail->path);

      // Remove the cache entry at the tail
      struct cache_entry *lru = dllist_remove_tail(cache);

      // Free the cache entry
      free_entry(lru);

      // Make sure the size counter for the num of entries is correct
      struct cache_entry *cur_entry = cache->head;

      int counter = 0;
      // Loop setting the next entry
      while(cur_entry != NULL) {
        struct cache_entry *next_entry = cur_entry->next;
        counter++;
        cur_entry = next_entry;
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

    // Find the cache entry
    struct cache_entry *entry = hashtable_get(cache->index, path);

    // If empty, return NULL
    if (entry == NULL) {
      return NULL;
    }
    // Move the entry to the head
    dllist_move_to_head(cache, entry);
    // Return the entry
    return entry;
}
