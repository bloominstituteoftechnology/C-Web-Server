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

free() - used for free_entry()
https://www.tutorialspoint.com/c_standard_library/c_function_free.htm
The C library function void free(void *ptr) deallocates the memory previously 
allocated by a call to calloc, malloc, or realloc. No return value.

realloc()
https://www.tutorialspoint.com/c_standard_library/c_function_realloc.htm
The C library function void *realloc(void *ptr, size_t size) attempts to 
resize the memory block pointed to by ptr that was previously allocated 
with a call to malloc or calloc.  Returns a pointer to newly allocated memory.

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

    //define cache entry with pointer to hold the variable
    struct cache_entry *cache_entry = malloc(sizeof(*cache_entry)); //returns a pointer

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

    /*
    in order to free the memory allocated to an entry, 
    we need to free the memory associated with each component
    of that entry. Reference using entry->component, use free 
    to deallocate the memory (no return value).
    */

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
    if (cache->head == NULL) {
        cache->head = cache->tail = ce; //the cache entry is the first and only entry
        ce->prev = ce->next = NULL; // there are no adjoining items in the list
    } else {
        cache->head->prev = ce; // the new entry is inserted in front of the head
        ce->next = cache->head; // the former head becomes the next relative to the new entry
        ce->prev = NULL; // because the new entry is now the head, it has no previous node
        cache->head = ce; // the new entry is set as the head
    }
}

/**
 * Move a cache entry to the head of the list
 */
void dllist_move_to_head(struct cache *cache, struct cache_entry *ce)
{
    if (ce != cache->head) { // we don't need to move the entry if it is already at the head
        if (ce == cache->tail) {
            // if the entry is the tail, we have to set a new tail to be the previous node and the next node to null
            cache->tail = ce->prev;
            cache->tail->next = NULL;

        } else {
            // We're neither the head nor the tail
            ce->prev->next = ce->next; // the next node relative to ce becomes the next node relative to the node previous to ce
            ce->next->prev = ce->prev; // the previous node relative to ce becomes the previous node relative to the next node to ce
        }

        ce->next = cache->head; // the head of the cache becomes ce's next node
        cache->head->prev = ce; // ce becomes the previous node relative to cache head
        ce->prev = NULL; // ce's previous node is null because ce is now the head
        cache->head = ce; // set ce as head
    }
}


/**
 * Removes the tail from the list and returns it
 * 
 * NOTE: does not deallocate the tail
 */
struct cache_entry *dllist_remove_tail(struct cache *cache)
{
    struct cache_entry *oldtail = cache->tail; //creates a pointer to the old tail

    cache->tail = oldtail->prev; // the cache tail becomes the old tail's previous node
    cache->tail->next = NULL; // the cache tail's next node becomes NULL

    cache->cur_size--; // decrement the size of the cache

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

    /*
    // A cache
    struct cache {
        struct hashtable *index;
        struct cache_entry *head, *tail; // Doubly-linked list
        int max_size; // Maxiumum number of entries
        int cur_size; // Current number of entries
        };
    */

    // create pointer to hold the cache 
    struct cache *cache = malloc(sizeof(*cache));
    // set the head
    cache->head = NULL;
    // set the tail
    cache->tail = NULL;
    // set up indexing with hashtable
    cache->index = hashtable_create(hashsize,NULL);
    // set the max size 
    cache->max_size = max_size;
    // set the current size
    cache->cur_size = 0;
    //return the cache
    return cache;
}

void cache_free(struct cache *cache)
{
    // create a pointer to a cur_entry variable and set it equal to the head
    struct cache_entry *cur_entry = cache->head; 
    // get rid of the index corresponding to the cache
    hashtable_destroy(cache->index);
    // 
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
    struct cache_entry *cache_entry = alloc_entry(path, content_type, content, content_length);

    // Insert the entry at the head of the doubly-linked list. 
    // Use dllist_insert_head(struct cache *cache, struct cache_entry *ce)
    dllist_insert_head(cache, cache_entry);
    
    // Store the entry in the hashtable as well, indexed by the entry's `path`.
    // hashtable_put uses a string as a key
    hashtable_put(cache->index, path, content);  // key is the path, value is the content  

    // Increment the current size of the cache.
    cache->cur_size++;

    // If the cache size is greater than the max size:
    if (cache->cur_size >= cache->max_size)
    {
    // Remove the entry from the hashtable, using the entry's `path` and the `hashtable_delete` function.
    hashtable_delete(cache->index, path);

    /* Remove the cache entry at the tail of the linked list (this is the
    least-recently used one)
     // Free the cache entry.
    */

    free_entry(dllist_remove_tail(cache));
    /*dlist_remove_tail returns a cache entry corresponding to the removed tail,
    which we can then enter into the free entry function which will deallocate
    memory and decrement the number of entries.
    Ensure the size counter for the number of entries in the cache is correct.
    */
    };
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    //create a cache_entry
    struct cache_entry *cache_entry;

    // Attempt to find the cache entry pointer by path in the hash table.
    cache_entry = hashtable_get(cache->index, path);
    // If not found, return NULL.
    if(cache_entry == NULL){
        return NULL;
    };
    
    // Move the cache entry to the head of the doubly-linked list.
    dllist_move_to_head(cache, cache_entry);

    // Return the cache entry pointer.
    return cache_entry;

}
