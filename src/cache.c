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
  struct cache_entry *an_entry = malloc(sizeof (struct cache_entry));
  an_entry->path = strdup(path);
  an_entry->content_type = strdup(content_type);
  an_entry->content = strdup(content);
  an_entry->content_length = strdup(content_length);
  return an_entry;
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
  struct cache *nu_cache = malloc(sizeof(struct cache));
  struct hashtable *ht = malloc(sizeof(struct hashtable));

  nu_cache->max_size = max_size;
  nu_cache->cur_size = hashsize;
  nu_cache->index = malloc(sizeof(struct hashtable));
  nu_cache->head = malloc(sizeof(struct cache_entry));
  nu_cache->tail = malloc(sizeof(struct cache_entry));
  nu_cache->index = hashtable_create(128, NULL);

  return nu_cache;
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
  struct cache_entry *an_entry = alloc_entry(path, content_type, content, content_length);
  if(cache->cur_size < cache->max_size){
    dllist_insert_head(cache, an_entry);
    hashtable_put(cache->index, an_entry->path, an_entry);
  } else if ( cache->cur_size >= cache->max_size ) {
    struct cache_entry *ol_yellow = dllist_remove_tail(cache);
    hashtable_delete(cache->index, an_entry->path);
    free_entry(ol_yellow);
    dllist_insert_head(cache, an_entry);
    hashtable_put(cache->index, an_entry->path, an_entry);
  } else {
    printf("Error performing PUT.");
  }
  return;
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    struct cache_entry *is_entry = hashtable_get(cache->index, path);
    if(is_entry == NULL){
      return NULL;
    }
    dllist_move_to_head(cache, is_entry);
    return is_entry;
}
