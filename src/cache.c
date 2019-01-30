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
	
	struct cache_entry *new_entry = malloc(sizeof(cache_entry));  //Allocate a new cache entry

        new_entry->path = path;
	new_entry->content_type = content_type;
	new_entry->content = content;
	new_entry->content_length = content_length;
        new_entry->prev = NULL;
        new_entry->next = NULL;	
	
	return new_entry;

}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
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

Allocate a new cache entry with the passed parameters.

Insert the entry at the head of the doubly-linked list.

Store the entry in the hashtable as well, indexed by the entry's path.

Increment the current size of the cache.

If the cache size is greater than the max size:

Remove the cache entry at the tail of the linked list.

Remove that same entry from the hashtable, using the entry's path and the hashtable_delete function.

Free the cache entry.

Ensure the size counter for the number of entries in the cache is correct.

struct cache {
    struct hashtable *index;
    struct cache_entry *head, *tail; // Doubly-linked list
    int max_size; // Maxiumum number of entries
    int cur_size; // Current number of entries
};

 */
void cache_put(struct cache *cache, char *path, char *content_type, void *content, int content_length)
{
		
	
	struct cache_entry *new_entry = alloc_entry(path, content_type, content, content_length); //Allocate a new cache entry
	
	dllist_insert_head(cache, new_entry);                //Insert the entry at the head of the doubly-linked list.

	hashtable_put(cache->index, new_entry->path, new_entry->content);       //Store the entry in the hashtable as well, indexed by the entry's path.
	
	cache->cur_size++;                                 //Increment the current size of the cache.
	
	if(cache->cur_size > cache->max_size){            //checking if the cache size is greater than the max size
		

		
		old_tail = dllist_remove_tail(cache);               //removing the cache entry at the tail of the linked list

		hashtable_delete(cache->index, old_tail->path);    //Removing that same entry from the hashtable

		free_entry(old_tail);                   //Free the cache entry

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
}
