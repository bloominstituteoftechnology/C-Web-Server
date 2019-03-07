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
    struct cache_entry *entry = malloc(sizeof(struct cache_entry));

    entry->content = malloc(content_length);
    memcpy(entry->content, content, content_length);

    entry->content_length = content_length;

    entry->content_type = malloc(strlen(content_type) + 1);
    strcpy(entry->content_type, content_type);

    entry->path = malloc(strlen(path) + 1);
    strcpy(entry->path, path);

    return entry;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    free(entry->content);
    free(entry->content_length);
    free(entry->content_type);
    free(entry->path);
    free(entry);
}

/**
 * Insert a cache entry at the head of the linked list
 */
void dllist_insert_head(struct cache *cache, struct cache_entry *ce)
{
    // Insert at the head of the list
    if (cache->head == NULL)
    {
        cache->head = cache->tail = ce;
        ce->prev = ce->next = NULL;
    }
    else
    {
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
    if (ce != cache->head)
    {
        if (ce == cache->tail)
        {
            // We're the tail
            cache->tail = ce->prev;
            cache->tail->next = NULL;
        }
        else
        {
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
    struct cache *cache = malloc(sizeof(struct cache));
    cache->cur_size = 0;
    cache->max_size = max_size;
    // hashtable_create(int size, int (*hashf)(void *, int, int));
    cache->index = hashtable_create(hashsize, NULL);

    return cache;
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
    //     Allocate a new cache entry with the passed parameters.
    // alloc_entry(char *path, char *content_type, void *content, int content_length)
    struct cache_entry *entry = alloc_entry(path, content_type, content, content_length);
    // Insert the entry at the head of the doubly-linked list.
    // void dllist_insert_head(struct cache *cache, struct cache_entry *ce)
    dllist_insert_head(cache, entry);
    // Store the entry in the hashtable as well, indexed by the entry's path.
    // hashtable_put(struct hashtable *ht, char *key, void *data);
    hashtable_put(cache->index, entry->path, entry->content);
    // Increment the current size of the cache.
    cache->cur_size++;
    // If the cache size is greater than the max size:
    if (cache->cur_size > cache->max_size)
    {
        // Remove the cache entry at the tail of the linked list.
        cache->tail->prev->next = NULL;
        // /Remove that same entry from the hashtable, using the entry's path and the hashtable_delete function.
        // void *hashtable_delete(struct hashtable *ht, char *key);
        hashtable_delete(cache->index, cache->tail->path);
        // Free the cache entry.
        free_entry(cache->tail);
        // Ensure the size counter for the number of entries in the cache is correct.
        cache->cur_size--;
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
    // Attempt to find the cache entry pointer by path in the hash table.
    // If not found, return NULL.
    // Move the cache entry to the head of the doubly-linked list.
    // Return the cache entry pointer.
}
