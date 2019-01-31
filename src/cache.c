#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"
#include <assert.h>

/**
 * Allocate a cache entry
 */
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    struct cache_entry *entry = malloc(sizeof(struct cache_entry));
    entry->path = strdup(path);
    entry->content_type = strdup(content_type);
    entry->content_length = content_length;
    entry->content = content;
    return entry;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    if (entry->path != NULL)             { free(entry->path); }
    if (entry->content_type != NULL)     { free(entry->content_type); }
    if (entry != NULL)                   { free(entry); }
    return;
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
    printf("\n\n we're inside move_to_head\n");
    if (ce != cache->head) {
        printf("we're not the head\n");
        if (ce == cache->tail) {
            // We're the tail
            printf("we're the tail\n");
            cache->tail = ce->prev;
            cache->tail->next = NULL;

        } else {
            // We're neither the head nor the tail
            printf("we're neither head nor tail\n");

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
    struct cache *new_cache = malloc(sizeof(struct cache));     //XXX
    struct hashtable *ht = hashtable_create(hashsize, NULL);    //XXX
    new_cache->index = ht;                                      //XXX

    new_cache->head = NULL;
    new_cache->tail = NULL;

    new_cache->max_size = max_size;
    new_cache->cur_size = 0;

    return new_cache;
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
// Allocate a new cache entry with the passed parameters.
    // printf("1\n");
   struct cache_entry *entry = alloc_entry(path, content_type, content, content_length);
// Insert the entry at the head of the doubly-linked list.
    // printf("2\n");
    dllist_insert_head(cache, entry);
// Store the entry in the hashtable as well, indexed by the entry's path.
    // printf("3\n");
    hashtable_put(cache->index, path, entry);
// Increment the current size of the cache.
    // printf("4\n");
    cache->cur_size++;
// If the cache size is greater than the max size:
    // printf("5\n");
    if (cache->cur_size > cache->max_size) {
    // Remove the cache entry at the tail of the linked list (this is the least-recently used one)
    // printf("cache->max_size: %d\n", cache->max_size);
    // printf("cache->cur_size: %d\n~~~~\n\n", cache->cur_size);
    // printf("6\n");
        struct cache_entry *oldtail = dllist_remove_tail(cache);
    // Remove the entry from the hashtable, using the entry's path and the hashtable_delete function.
    // printf("7\n");
        hashtable_delete(cache->index, oldtail->path);
    // Free the cache entry.
    // printf("8\n");
        // printf("oldtail: %s\n", oldtail->path);
        free_entry(oldtail);
        // printf("oldtail: %s\n", oldtail);
    // Ensure the size counter for the number of entries in the cache is correct.
    // printf("9\n");
    // printf("cache->max_size: %d\n", cache->max_size);
    // printf("cache->cur_size: %d\n", cache->cur_size);
        // assert(cache->cur_size <= cache->max_size);
    }

}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
// Attempt to find the cache entry pointer by path in the hash table.
    printf("1~\n");
    printf("cache_get path: %s\n", path);
    if (path == NULL){
        return NULL;
    }
    struct cache_entry *entry = hashtable_get(cache->index, path);
    if (entry == cache->tail){ printf("works here\n");}
    else {printf("didn't work here\n");}
// If not found, return NULL.
    printf("2~\n");
    if (entry == NULL){
    printf("3~\n");
        return entry;
    }
// Move the cache entry to the head of the doubly-linked list.
    printf("4~\n");
    printf("ENTRY: %s\n", entry);
    printf("BEFORE HEAD: %s\n", cache->head->path);
    printf("BEFORE TAIL: %s\n", cache->tail);
    dllist_move_to_head(cache, entry);
    printf("HEAD: %s\n", cache->head->path);
    printf("TAIL: %s\n", cache->tail->path);

// Return the cache entry pointer.
    printf("5~\n");
    return entry;
}

// struct cache_entry *cache_get(struct cache *cache, char *path)
// {
//     if(!hashtable_get(cache->index, path)){
//         return NULL;

//     }else{
//         struct cache_entry *ce = hashtable_get(cache->index, path);
//         dllist_move_to_head(cache, ce);
//         return ce; 
//         }
// }