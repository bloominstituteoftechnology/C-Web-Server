#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "minunit.h"
#include "../cache.h"
#include "../hashtable.h"

char *test_cache_create()
{
  int max_size = 10;
  int hash_size = 0;

  struct cache *cache = cache_create(max_size, hash_size);

  // Check that each field of the cache struct was initialized to the proper value
  mu_assert(cache, "Your cache_create function did not return a valid pointer to the created cache");
  mu_assert(cache->head == NULL, "The head pointer of the cache should be initialized to NULL");
  mu_assert(cache->tail == NULL, "The tail pointer of the cache should be initialized to NULL");
  mu_assert(cache->cur_size == 0, "The cur_size field of the cache should be initialized to 0");
  mu_assert(cache->max_size == max_size, "The max_size field of the cache was not initialized to the expected value");
  mu_assert(cache->index != NULL, "The index field of the cache was not initialized");

  cache_free(cache);

  return NULL;
}

char *test_cache_alloc_entry()
{
  char *path = "/bazz/lurman.html";
  char *content_type = "text/html";
  char *content = "<head>Bazz Lurman</head>";
  int content_len = strlen(content) + 1; // +1 to include the \0

  struct cache_entry *ce = alloc_entry(path, content_type, content, content_len);

  // Check that the allocated entry was initialized with expected values
  mu_assert(check_strings(ce->path, path) == 0, "Your alloc_entry function did not allocate the path field to the expected string");
  mu_assert(check_strings(ce->content_type, content_type) == 0, "Your alloc_entry function did not allocate the content_type field to the expected string");
  mu_assert(check_strings(ce->content, content) == 0, "Your alloc_entry function did not allocate the content field to the expected string");
  mu_assert(ce->content_length == content_len, "Your alloc_entry function did not allocate the content_length field to the expected length")

  free_entry(ce);

  return NULL;
}

char *test_cache_put()
{
  // Create a cache with 3 slots
  struct cache *cache = cache_create(3, 0);
  // Create 4 test entries
  struct cache_entry *test_entry_1 = alloc_entry("/1", "text/plain", "1", 2);
  struct cache_entry *test_entry_2 = alloc_entry("/2", "text/html", "2", 2);
  struct cache_entry *test_entry_3 = alloc_entry("/3", "application/json", "3", 2);
  struct cache_entry *test_entry_4 = alloc_entry("/4", "image/png", "4", 2);

  // Add in a single entry to the cache
  cache_put(cache, test_entry_1->path, test_entry_1->content_type, test_entry_1->content, test_entry_1->content_length);
  // Check that the cache is handling a single entry as expected
  mu_assert(cache->cur_size == 1, "Your cache_put function did not correctly increment the cur_size field when adding a new cache entry");
  mu_assert(cache->head->prev == NULL && cache->tail->next == NULL, "The head and tail of your cache should have NULL prev and next pointers when a new entry is put in an empty cache");
  mu_assert(check_cache_entries(cache->head, test_entry_1) == 0, "Your cache_put function did not put an entry into the head of the empty cache with the expected form");
  mu_assert(check_cache_entries(cache->tail, test_entry_1) == 0, "Your cache_put function did not put an entry into the tail of the empty cache with the expected form")
  mu_assert(check_cache_entries(hashtable_get(cache->index, "/1"), test_entry_1) == 0, "Your cache_put function did not put the expected entry into the hashtable");

  // Add in a second entry to the cache
  cache_put(cache, test_entry_2->path, test_entry_2->content_type, test_entry_2->content, test_entry_2->content_length);
  // Check that the cache is handling both entries as expected
  mu_assert(cache->cur_size == 2, "Your cache_put function did not correctly increment the cur_size field when adding a new cache entry");
  mu_assert(check_cache_entries(cache->head, test_entry_2) == 0, "Your cache_put function did not put an entry into the head of the cache with the expected form");
  mu_assert(check_cache_entries(cache->tail, test_entry_1) == 0, "Your cache_put function did not move the oldest entry in the cache to the tail of the cache");
  mu_assert(check_cache_entries(cache->head->next, test_entry_1) == 0, "Your cache_put function did not correctly set the head->next pointer of the cache");
  mu_assert(check_cache_entries(hashtable_get(cache->index, "/2"), test_entry_2) == 0, "Your cache_put function did not put the expected entry into the hashtable");

  // Add in a third entry to the cache
  cache_put(cache, test_entry_3->path, test_entry_3->content_type, test_entry_3->content, test_entry_3->content_length);
  // Check that the cache is handling all three entries as expected
  mu_assert(cache->cur_size == 3, "Your cache_put function did not correctly increment the cur_size field when adding a new cache entry");
  mu_assert(check_cache_entries(cache->head, test_entry_3) == 0, "Your cache_put function did not correctly update the head pointer of the cache");
  mu_assert(check_cache_entries(cache->head->next, test_entry_2) == 0, "Your cache_put function did not update the head->next pointer to point to the old head");
  mu_assert(check_cache_entries(cache->head->next->prev, test_entry_3) == 0, "Your cache_put function did not update the head->next->prev pointer to point to the new head entry");
  mu_assert(check_cache_entries(cache->head->next->next, test_entry_1) == 0, "Your cache_put function did not update the head->next->next pointer to point to the tail entry");
  mu_assert(check_cache_entries(cache->tail->prev, test_entry_2) == 0, "Your cache_put function did not update the tail->prev pointer to poin to the second-to-last entry");
  mu_assert(check_cache_entries(cache->tail, test_entry_1) == 0, "Your cache_put function did not correctly update the tail pointer of the cache"); 

  // Add in a fourth entry to the cache
  cache_put(cache, test_entry_4->path, test_entry_4->content_type, test_entry_4->content, test_entry_4->content_length);
  // Check that the cache removed the oldest entry and is handling the three most-recent entries correctly
  mu_assert(cache->cur_size == 3, "Your cache_put function did not correctly handle the cur_size field when adding a new cache entry to a full cache");
  mu_assert(check_cache_entries(cache->head, test_entry_4) == 0, "Your cache_put function did not correctly handle adding a new entry to an already-full cache");
  mu_assert(check_cache_entries(cache->head->next, test_entry_3) == 0, "Your cache_put function did not update the head->next pointer to point to the old head");
  mu_assert(check_cache_entries(cache->head->next->prev, test_entry_4) == 0, "Your cache_put function did not update the head->next->prev pointer to point to the new head entry");
  mu_assert(check_cache_entries(cache->head->next->next, test_entry_2) == 0, "Your cache_put function did not update the head->next->next pointer to point to the tail entry");
  mu_assert(check_cache_entries(cache->tail->prev, test_entry_3) == 0, "Your cache_put function did not update the tail->prev pointer to poin to the second-to-last entry");
  mu_assert(check_cache_entries(cache->tail, test_entry_2) == 0, "Your cache_put function did not correctly handle the tail of an already-full cache");

  cache_free(cache);

  return NULL;
}

char *test_cache_get()
{
  // Create a cache with 2 slots
  struct cache *cache = cache_create(2, 0);
  // Create 3 test entries
  struct cache_entry *test_entry_1 = alloc_entry("/1", "text/plain", "1", 2);
  struct cache_entry *test_entry_2 = alloc_entry("/2", "text/html", "2", 2);
  struct cache_entry *test_entry_3 = alloc_entry("/3", "application/json", "3", 2);

  struct cache_entry *entry;

  // Insert an entry into the cache, then retrieve it
  cache_put(cache, test_entry_1->path, test_entry_1->content_type, test_entry_1->content, test_entry_1->content_length);
  entry = cache_get(cache, test_entry_1->path);
  // Check that the retrieved entry's values match the values of the inserted entry
  mu_assert(check_cache_entries(entry, test_entry_1) == 0, "Your cache_get function did not retrieve the newly-added cache entry when there was 1 entry in the cache");

  // Insert another entry into the cache, then retrieve it
  cache_put(cache, test_entry_2->path, test_entry_2->content_type, test_entry_2->content, test_entry_2->content_length);
  entry = cache_get(cache, test_entry_2->path);
  // Check the retrieved entry's values and also check that the entries are ordered correctly in the cache
  mu_assert(check_cache_entries(entry, test_entry_2) == 0, "Your cache_get function did not retrieve the newly-added cache entry when there were 2 entries in the cache");
  mu_assert(check_cache_entries(cache->head, test_entry_2) == 0, "Your cache_get function did not update the head pointer to point to the newly-added entry when there are 2 entries");
  mu_assert(check_cache_entries(cache->tail, test_entry_1) == 0, "Your cache_get function did not move the oldest entry to the tail of the cache");

  // Insert a third entry into the cache, then retrieve it
  cache_put(cache, test_entry_3->path, test_entry_3->content_type, test_entry_3->content, test_entry_3->content_length);
  entry = cache_get(cache, test_entry_3->path);
  // Check the retrieved entry's values and also check that the entries are ordered correctly in the cache
  mu_assert(check_cache_entries(entry, test_entry_3) == 0, "Your cache_get function did not retrieve the newly-added cache entry when there were 3 entries in the cache");
  mu_assert(check_cache_entries(cache->head, test_entry_3) == 0, "Your cache_get function did not update the head pointer to point to the newly-added entry when there are 3 entries");
  mu_assert(check_cache_entries(cache->tail, test_entry_2) == 0, "Your cache_get function did not move the oldest entry to the tail of the cache when there are 3 entries");
  // Check that the oldest cache entry cannot be retrieved
  mu_assert(cache_get(cache, test_entry_1->path) == NULL, "Your cache_get function did not remove the oldest entry from the cache");

  // Retrieve the oldest entry in the cache
  entry = cache_get(cache, test_entry_2->path);
  // Check that the most-recently accessed entry has been moved to the head of the cache
  mu_assert(check_cache_entries(cache->head, test_entry_2) == 0, "Your cache_get function did not move the most-recently retrieved entry to the head of the cache");
  mu_assert(check_cache_entries(cache->tail, test_entry_3) == 0, "Your cache_get function did not move the oldest entry to the tail of the cache");

  cache_free(cache);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_cache_create);
  mu_run_test(test_cache_alloc_entry);
  mu_run_test(test_cache_put);
  mu_run_test(test_cache_get);

  return NULL;
}

RUN_TESTS(all_tests)
