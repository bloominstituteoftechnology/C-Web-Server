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
    struct cache_entry *new_entry = malloc(sizeof(struct cache_entry));
    new_entry->path = path;
    new_entry->content_type = content_type;
    new_entry->content_length = content_length;
    new_entry->content = content;

    return new_entry;
}
//look at cache entry in cache.h to understand how to initialize alloc_entry
// struct cache_entry {
//     char *path;   // Endpoint path--key to the cache
//     char *content_type;
//     int content_length;
//     void *content;

//     struct cache_entry *prev, *next; // Doubly-linked list
// };

    // struct cache *new_cache = malloc(sizeof(struct cache));
    // struct hashtable *new_ht = hashtable_create(hashsize, NULL);
    // new_cache->index = new_ht;
    // new_cache->head = NULL;
    // new_cache->tail = NULL;
    // new_cache->max_size = max_size;
    // new_cache->cur_size = 0;

    // return new_cache;

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
    struct cache *new_cache = malloc(sizeof(struct cache));
    struct hashtable *new_ht = hashtable_create(hashsize, NULL);
    new_cache->index = new_ht;
    new_cache->head = NULL;
    new_cache->tail = NULL;
    new_cache->max_size = max_size;
    new_cache->cur_size = 0;

    return new_cache;
}
//look at the cache struct in cache.h file to understand how to initialize cache_create
// struct cache {
//     struct hashtable *index;
//     struct cache_entry *head, *tail; // Doubly-linked list
//     int max_size; // Maxiumum number of entries
//     int cur_size; // Current number of entries
// };

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
// Allocate a new cache entry with the passed parameters.
//struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
   struct cache_entry* new_entry = alloc_entry(path, content_type, content, content_length);
// Insert the entry at the head of the doubly-linked list.
//dllist_insert_head(struct cache *cache, struct cache_entry *ce)
    dllist_insert_head(cache, new_entry);
// Store the entry in the hashtable as well, indexed by the entry's path.
//*hashtable_put(struct hashtable *ht, char *key, void *data)
    hashtable_put(cache->index, path, new_entry);
// Increment the current size of the cache. 
    cache->cur_size++;
// If the cache size is greater than the max size:
// Remove the entry from the hashtable, using the entry's path and the hashtable_delete function.
// Remove the cache entry at the tail of the linked list.
// Free the cache entry.
// Ensure the size counter for the number of entries in the cache is correct.
    if (cache->cur_size > cache->max_size) {
//*hashtable_delete(struct hashtable *ht, char *key)
//cache_entry *dllist_remove_tail(struct cache *cache)
        hashtable_delete(cache->index, cache->tail->path);
        free_entry(dllist_remove_tail(cache));
        cache->cur_size = cache->max_size;
    }
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
//maybe create a pointer to the cache get entry
    void* get_entry = hashtable_get(cache->index, path);
// Attempt to find the cache entry pointer by path in the hash table.
// If not found, return NULL.
//check hashtable_get in hashtable.c to understand function
// Move the cache entry to the head of the doubly-linked list.
//void dllist_move_to_head(struct cache *cache, struct cache_entry *ce)
// Return the cache entry pointer.
    if (get_entry == 0) {
        return NULL;
    } 
        dllist_move_to_head(cache, get_entry);
        return get_entry;
    
}
