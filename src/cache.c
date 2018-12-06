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

    //copy and make a new pointer so that it cant be messed with 
    struct cache_entry *new = malloc(sizeof(struct cache_entry));
    // char *copy = strcpy(//something, content);//look at string copy and dupe

    new->content = content;//malloc
    new->content_length = content_length;
    new->path = path;
    new->content_type = content_type;

    //return 
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    
    //--------------------NULL check? 
    
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
    struct cache *foobar = malloc(sizeof(foobar));
    
    foobar->index = hashtable_create(hashsize, NULL);//------------------max_size?
    foobar->head = NULL;
    foobar->tail = NULL;
    foobar->max_size = max_size;
    foobar->cur_size = hashsize;
    return foobar;
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
//    * Allocate a new cache entry with the passed parameters.
    struct cache_entry *new_entry = alloc_entry(path, content_type, content, content_length);

//    * Insert the entry at the head of the doubly-linked list.
    dllist_insert_head(cache, new_entry);
//    * Store the entry in the hashtable as well, indexed by the entry's `path`.
    hashtable_put(cache->index, path, content);
//    * Increment the current size of the cache.
    cache->cur_size++;
//    * If the cache size is greater than the max size:
    if(cache->cur_size > cache->max_size)
    {
//      * Remove the entry from the hashtable, using the entry's `path` and the `hashtable_delete` function.
        hashtable_delete(cache->index, path);//path is wrong? cache->index->path?
//      * Remove the cache entry at the tail of the linked list.
        struct cache_entry *old_entry = dllist_remove_tail(cache);//------------ this deletes the cache entry and returns the pointer which is what should be deleted in the next step. 
//      * Free the cache entry.
        free_entry(old_entry); //-------------------delete the returnd value from 161
    }

//      * Ensure the size counter for the number of entries in the cache is correct.
    if(cache->cur_size > cache->max_size){
        printf("Something went wrong. I don't know what but it has to do with the current size of the cache being greater than the max size of the cache.");
    } else {
        printf("sucessfully saved to cache.");
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
    printf("inside cache_get\n");
//    * Attempt to find the cache entry pointer by `path` in the hash table.
    struct cache_entry *found_entry = hashtable_get(cache->index, path);//-----this should be index because it the refrence
    printf("after hashtable_get\n");
//    * If not found, return `NULL`.
    if(found_entry == NULL){
    printf("NULL\n");
        return NULL;
    }
//    * Move the cache entry to the head of the doubly-linked list.
    printf("before cache_put\n");

    dllist_move_to_head(cache, found_entry);//------------use this instead of the below function cause it is already written ----------
    // cache_put(cache, found_entry->path, found_entry->content_type, found_entry->content, found_entry->content_length);
    printf("after cache_put\n");
//    * Return the cache entry pointer.
    return cache->head;
}
