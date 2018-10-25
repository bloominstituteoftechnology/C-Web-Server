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
    printf("\n\nCACHING NOW!\n\n");
    // grab the memory for the struct
    struct cache_entry *cache_entry = malloc(sizeof *cache_entry);

    // grab memory for the path string
    cache_entry->path = malloc(strlen(path) + 1);
    strcpy(cache_entry->path, path);

    // grab memory for the content_type string
    cache_entry->content_type = malloc(strlen(content_type) + 1);
    strcpy(cache_entry->content_type, content_type);

    // grab memory for the content string
    cache_entry->content = malloc(strlen(content) + 1);
    // memcpy to handle binary data
    memcpy(cache_entry->content, content, content_length);

    // numbers are easy - don't even have to free
    cache_entry->content_length = content_length;

    return cache_entry;
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
}

/**
 * Deallocate a cache entry
 */
// void free_entry(void *v_ent, void *varg)
void free_entry(struct cache_entry *entry)
{
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
    // get the memory
    struct cache *cache = malloc(sizeof *cache);
    // initialize to starting values
    cache->index = hashtable_create(hashsize, NULL); // hashtable does its own malloc
    cache->head = NULL;
    cache->tail = NULL;
    cache->max_size = max_size;
    cache->cur_size = 0;

    return cache;
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
    struct cache_entry *cache_entry = alloc_entry(path, content_type, content, content_length);

    // Insert the entry at the head of the doubly-linked list.
    dllist_insert_head(cache, cache_entry);

    // Store the entry in the hashtable as well, indexed by the entry's path.
    hashtable_put(cache->index, cache_entry->path, cache_entry);

    // Increment the current size of the cache.
    cache->cur_size++;

    // If the cache size is greater than the max size:
    if (cache->cur_size > cache->max_size)
    {
        // Remove the entry from the hashtable, using the entry's path and the hashtable_delete function.
        cache_entry = dllist_remove_tail(cache);
        // Remove the cache entry at the tail of the linked list.
        hashtable_delete(cache->index, cache_entry->path);
        // Free the cache entry.
        free_entry(cache_entry);
    }

    /// Ensure the size counter for the number of entries in the cache is correct.
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    // declare our cache_entry pointer variable
    struct cache_entry *cache_entry;

    // Attempt to find the cache entry pointer by path in the hash table.
    cache_entry = hashtable_get(cache->index, path);

    // If not found, return NULL.
    if (cache_entry == NULL)
    {
        return NULL;
    }

    // Move the cache entry to the head of the doubly - linked list.
    dllist_move_to_head(cache, cache_entry);

    // Return the cache_entry pointer.
    return cache_entry;
}
