#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

struct hashtable {
    int size; // Read-only
    int num_entries; // Read-only
    float load; // Read-only
    struct llist **bucket;
    int (*hashf)(void *data, int data_size, int bucket_count);
};

extern struct hashtable *hashtable_create(int size, int (*hashf)(void *, int, int));
extern void hashtable_destroy(struct hashtable *ht);
extern void *hashtable_put(struct hashtable *ht, char *key, void *data);
extern void *hashtable_put_bin(struct hashtable *ht, void *key, int key_size, void *data);
extern void *hashtable_get(struct hashtable *ht, char *key);
extern void *hashtable_get_bin(struct hashtable *ht, void *key, int key_size);
extern void *hashtable_delete(struct hashtable *ht, char *key);
extern void *hashtable_delete_bin(struct hashtable *ht, void *key, int key_size);
extern void hashtable_foreach(struct hashtable *ht, void (*f)(void *, void *), void *arg);

#endif
