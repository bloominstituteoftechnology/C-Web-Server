#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"

/**
 * Allocate a cache entry
 *///                                                           filedata->data  file_data->size
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    // printf("made it to alloc entry\n");
    // int content_length_static = malloc(sizeof(content_length);
    struct cache_entry *ce = malloc(sizeof(struct cache_entry));
    ce->path = strdup(path);
    ce->content_type = strdup(content_type);
    printf("made it to alloc entry <----------------------------o o\n");
    ce->content_length = content_length;
    ce->content = malloc(content_length + 1); //refactor later to only increment on text
    printf("content <======= %p\n", ce->content);
    memcpy(ce->content, content, content_length + 1); //sizeof(ce->content)
    printf("content <======= %p\n", ce->content);
    printf("right before return\n"); 
    // I think memory for ce->content needs to be allocated first, but not sure
    return ce;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry) 
{
//  if (entry->content != NULL) {
    
// } ... check everything if it's allocated for if not don't free or stuff hits the fan
    free(entry->path);
    free(entry->content_type);
    free(entry->content_length);
    free(entry->content);
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
    /*
    c->index = some _function_ which returns a hashtable;
    The model we are following: 
    struct cache {
        struct hashtable *index;
        struct cache_entry *head, *tail; // Doubly-linked list
        int max_size; // Maxiumum number of entries
        int cur_size; // Current number of entries
    };

    TODO: Initialize index as a hashtable. - 
    Initialize head/tail. Should be NULL for an empty cache.
    */
    struct cache *c = malloc(sizeof(struct cache));
    c->index = hashtable_create(hashsize, NULL);
    c->head = NULL;
    c->tail = NULL;
    // passing NULL to second argument of `hashtable_create` makes it use a default 
    // hashing function
    // You can instead pass a hash function of your own instead, though that is
    // way outside the scope of MVP, or even this project

    
    
    c->max_size = max_size;
    c->cur_size = 0;
    return c;

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
//    * Allocate a new cache entry with the passed parameters.
    struct cache_entry *ce = alloc_entry(path, content_type, content, content_length);
//    * Insert the entry at the head of the doubly-linked list.
    dllist_insert_head(cache, ce);
//    * Store the entry in the hashtable as well, indexed by the entry's `path`.
    hashtable_put(cache->index,ce->path, ce->content);

//    * Increment the current size of the cache.
    cache->cur_size++;
//    * If the cache size is greater than the max size:
    if (cache->cur_size > cache->max_size) {
//      * Remove the entry from the hashtable, using the entry's `path` and the `hashtable_delete` function.
        hashtable_delete(cache->index, cache->tail->path);
        // We're not deleting the entry we just put
//      * Remove the cache entry at the tail of the linked list.
        dllist_remove_tail(cache);
//      * Free the cache entry.
        free_entry(cache->tail);
//      * Ensure the size counter for the number of entries in the cache is correct.
        cache->cur_size--;
    }


    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
//    * Attempt to find the cache entry pointer by `path` in the hash table.
//    * If not found, return `NULL`.
        struct cache_entry *in_hash = NULL;
        in_hash = hashtable_get(cache->index, path);
        if (in_hash = NULL) {
            return NULL;
        }
//    * Move the cache entry to the head of the doubly-linked list.
        dllist_move_to_head(cache, in_hash);
//    in_hash has the node you're looking for. you can use that
//    to make it the head of the list. look at some of the functions
//     above that can help you do that.
// I thought it just returns the data is the data the cache->entry pointer?
        // currently i'm looking at hashtable_gets return which doesn't look like a cache entry to me 
        //so that's why it's taking me a bit to move on from here
//    * Return the cache entry pointer.

// return n->data; it's going to be a ht- entry ok that makes enoguh sense to me

// It will here
        return in_hash;
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
}
