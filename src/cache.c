#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hashtable.h"
#include "cache.h"

/**
 * Allocate a cache entry
 */
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    struct cache_entry *entry = malloc(sizeof(*entry));
    time_t ltime = time(NULL);
    entry->path = strdup(path);
    entry->content_type = strdup(content_type);
    entry->content = malloc(content_length);
    memcpy(entry->content, content, content_length);
    entry->content_length = content_length;
    entry->created_at = strdup(asctime(localtime(&ltime)));
    entry->prev = NULL;
    entry->next = NULL;

    return entry;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    free(entry->path);
    free(entry->content_type);
    free(entry->content);
    free(entry->created_at);
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
    struct cache *cache = malloc(sizeof(*cache));
    cache->index = hashtable_create(hashsize, NULL);
    cache->head = NULL;
    cache->tail = NULL;
    cache->max_size = max_size;
    cache->cur_size = 0;

    return cache;
}

void cache_free(struct cache *cache)
{
    hashtable_destroy(cache->index);
    struct cache_entry *current_entry = cache->head;

    while (current_entry != NULL)
    {
        struct cache_entry *next_entry = current_entry->next;
        free_entry(current_entry);
        current_entry = next_entry;
    }

    free(cache);
}

void cache_delete(struct cache *cache, struct cache_entry *entry)
{
    if (cache->head == entry)
    {
        printf("%s\n", entry->next->content_type);
        // cache->head = cache->head->next;
        // cache->head->prev = NULL;
    }
    // if (cache->tail == entry)
    // {
    //     printf("TAIL\n");
    //     dllist_remove_tail(cache);
    // }
    // else
    // {
    //     printf("NEITHER\n");
    //     entry->prev->next = entry->next;
    //     entry->next->prev = entry->prev;
    // }

    printf("GOT HERE\n");
    // hashtable_delete(cache->index, entry->path);
    // free_entry(entry);
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
    struct cache_entry *entry = alloc_entry(path, content_type, content, content_length);

    dllist_insert_head(cache, entry);
    hashtable_put(cache->index, entry->path, entry);
    cache->cur_size++;

    while (cache->cur_size > cache->max_size)
    {
        entry = dllist_remove_tail(cache);
        hashtable_delete(cache->index, entry->path);
        free_entry(entry);
    }
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    struct cache_entry *entry = hashtable_get(cache->index, path);

    if (entry == NULL)
    {
        return NULL;
    }

    // struct tm current, created_at;

    // memset(&current, 0, sizeof(struct tm));
    // memset(&created_at, 0, sizeof(struct tm));

    // time_t tcurrent = time(NULL);
    // time_t tcreated_at;

    // strptime(asctime(localtime(&tcurrent)), "%a %b %d %H:%M:%S %Y", &current);
    // strptime(entry->created_at, "%a %b %d %H:%M:%S %Y", &created_at);

    // tcurrent = mktime(&current);
    // tcreated_at = mktime(&created_at);

    // if (1)
    // {
    //     cache_delete(cache, entry);
    //     return NULL;
    // }

    dllist_move_to_head(cache, entry);
    return entry;
}
