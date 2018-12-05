#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"

/*
We need a hash table to quickly look up cache entries by key hashtable.c
We need a doubly linked list to store cache entries in memory list.c
    - makes it easy to add and remove items 
    - head is most recently used, tail is least recently used (added/accessed),
     tail at list->next == NULL

malloc()
https://www.tutorialspoint.com/c_standard_library/c_function_malloc.htm
The C library function void *malloc(size_t size) allocates the requested 
memory and returns a pointer to it.

strlen()
https://www.tutorialspoint.com/c_standard_library/c_function_strlen.htm
The C library function size_t strlen(const char *str) computes the length of 
the string str up to, but not including the terminating null character.

strcpy()
https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm
The C library function char *strcpy(char *dest, const char *src) copies 
the string pointed to, by src to dest.

memcpy()
https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm
The C library function void *memcpy(void *str1, const void *str2, size_t n) 
copies n characters from memory area str2 to memory area str1.

*/

/**
 * Allocate a cache entry
 */
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    /* this function will allocate space for path, content_type, content, and content_length.
    after allocating the space for each, the data for each entry will be copied into the spaces.
    make use of malloc(), strlen(), strcpy(), and memcpy()
    */

    //define cache entry
    struct cache_entry *cache_entry = malloc(sizeof(*cache_entry));

    //set content_length
    cache_entry->content_length = content_length;

    //space for content (malloc(content_length))
    cache_entry->content = malloc(content_length);
    //copy content into newly created space using memcpy()
    //copies "content_length" characters from "content" into newly created space "cache_entry->content_length"
    memcpy(cache_entry->content, content, content_length); 
    
    // use strlen to get the size of path and content type
    int path_size = strlen(path) + 1; //+1 because of the null terminator \0
    int content_type_size  = strlen(content_type) +1; //+1 because of the null terminator \0

    // create space for path with malloc()
    cache_entry->path = malloc(path_size);
    // copy "path" into newly created space using strcpy()
    strcpy(cache_entry->path, path);
    
    //space for content_type with malloc()
    cache_entry->content_type = malloc(content_type_size);
    //copy "content_type" into newly created space using strcpy()
    strcpy(cache_entry->content_type, content_type);

    return cache_entry;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    //get pointer to least recently used (tail)

    //remove the tail

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
 */
void cache_put(struct cache *cache, char *path, char *content_type, void *content, int content_length)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // access item with key

    // move to head


}
