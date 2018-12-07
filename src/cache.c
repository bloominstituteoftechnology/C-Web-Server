#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"

/**
 * Allocate a cache entry
 LRU cache stuff
 */
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    struct cache_entry *entry = malloc(sizeof *entry);

    entry->path = malloc(strlen(path) + 1);
    strcpy(entry->path, path);

    entry->content_type = malloc(strlen(content_type) + 1);
    strcpy(entry->content_type, content_type);

    entry->content_length = content_length;

    entry->content = malloc(content_length);
    memcpy(entry->content, content, sizeof content_length);

    //copy over contents from the actual varibales to the pointers of the struct.




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
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    struct cache* created_cache = malloc(sizeof(struct cache));
    struct hashtable *hash = hashtable_create(hashsize, NULL);

    created_cache->index = hash;
    created_cache->head = NULL;
    created_cache->tail = NULL;
    created_cache->cur_size = 0;
    created_cache->max_size = max_size;

    return created_cache;
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
    // Allocate a new cache entry with the passed parameters.
    struct cache_entry *new_dll = alloc_entry(path, content_type, content_length, content);
    // insert new element into doubly linked list at the head
    dllist_insert_head(cache, new_dll);
    //put entry(now as a dll) into hashtable
    hashtable_put(cache->index, path, new_dll);
    //increase size of cache for each new entry.
    //Ensuring the size counter for the number of entries in the cache is correct.
    cache->cur_size++;
    //If the cache size is greater than the max size:
    struct cache_entry *tail;
    while(cache->cur_size > cache->max_size){
      //Remove the entry from the hashtable, using the entry's path and the hashtable_delete function.
      tail = dllist_remove_tail(cache);
      hashtable_delete(cache->index, tail->path);
      //Remove the cache entry at the tail of the linked list.

      //Free the cache entry.
      free_entry(new_dll);
    }

}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
  //Attempt to find the cache entry pointer by path in the hash table.
  struct cache_entry *find = hashtable_get(cache->index, path);

  if(find == NULL){
    //If not found, return NULL.
    return NULL;
  } else {
    //Move the cache entry to the head of the doubly-linked list.
    dllist_move_to_head(cache, find);
    //Return the cache entry pointer.
    return find;
  }
}
