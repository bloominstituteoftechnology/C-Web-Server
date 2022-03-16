#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"
#include "llist.h"

/**
 * Allocate a cache entry
 */
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    struct cache_entry *ret = malloc(sizeof(struct cache_entry));
    ret->path = path;
    ret->content = (char*)content;
    ret->content_type = content_type;
    ret->content_length = content_length;

    // ret->prev = malloc(sizeof(struct cache_entry));
    // ret->next = malloc(sizeof(struct cache_entry));

    ret->prev = NULL;
    ret->next = NULL;
    
    ret->created_at = time(NULL);

    return ret;
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
    struct cache *ret = malloc(sizeof(struct cache));
    ret->max_size = max_size;
    ret->cur_size = 0;
    ret->head = NULL;
    ret->tail = NULL;

    ret->index = hashtable_create(hashsize, NULL);

    return ret;
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

    // 1. Allocate a new cache entry with the passed parameters.
    // 2. Insert the entry at the head of the doubly-linked list.
    cache->cur_size++;

    printf("in cache_put(), %s %s %s %d\n", path, content_type, (char *)content, content_length);

    struct cache_entry *new_entry = alloc_entry(path, content_type, (char *)content, content_length);

    // head <- new
    // new -> head.nxt
    // head -> new
    // new <- head.nxt

    // struct cache_entry *head = cache->head;      //이렇게 head 선언하고 head = new_entry, tail = head 이런식으로 하니까 동작 안 함..
    // struct cache_entry *tail = cache->tail;

    if (cache->head == NULL)
    {
        cache->head = new_entry;
        cache->tail = new_entry;
    }
    else
    {
        new_entry->prev = NULL;
        new_entry->next = cache->head;
        cache->head->prev = new_entry;
        cache->head = new_entry;
    }

    printf("content = %s \n", (char *)cache->head->content);

    // 3. Store the entry in the hashtable as well, indexed by the entry's path
    struct hashtable *ht = cache->index;
    hashtable_put(ht, path, new_entry);

    //printf("path = %s\n, retrieved from hashtable = %s\n", path, (char*)((struct cache_entry*)(hashtable_get(ht, path)))->content);

    // 4. Increment the current size of the cache.
    // add_entry_count(ht,1);
    ht->num_entries += 1;
    ht->load = (float)ht->num_entries / ht->size;

    // 5. If the cache size is greater than the max size
    //      Remove the cache entry at the tail of the linked list.
    //      Remove that same entry from the hashtable, using the entry's path and the hashtable_delete function.
    //      Free the cache entry.
    //      Ensure the size counter for the number of entries in the cache is correct. => ***** how ? synchronized? mutex?*******
    if (cache->cur_size > cache->max_size)
    {
        struct cache_entry *old_tail = dllist_remove_tail(cache);
        hashtable_delete(ht, path);
        free_entry(old_tail);

        // TODO: 동기화 맞추기 구현해야 함
    }
}

// CONFUSE: 이 함수 쓰이나??
/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    // 1. Attempt to find the cache entry pointer by path in the hash table.
    struct hashtable *ht = cache->index;
    void *data = hashtable_get(ht, path); //리턴이 void * = 만능?

    // 2. If not found, return NULL
    if (data == NULL)
    {
        return NULL;
    }
    else
    {
        data = (char *)data;

        struct cache_entry *target = cache->head;

        for (; target != cache->tail && strcmp(target->path, data) != 0; target = target->next)
        { // CONFUSE: 맞나??
        }

        // 3. Move the cache entry to the head of the doubly-linked list.
        dllist_move_to_head(cache, target);

        // 4. Return the cache entry pointer.
        return target;
    }
}
