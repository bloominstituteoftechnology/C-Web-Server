#ifndef _LLIST_H_
#define _LLIST_H_

struct llist {
	struct llist_node *head;
	int count;
};

extern struct llist *llist_create(void);
extern void llist_destroy(struct llist *llist);
extern void *llist_insert(struct llist *llist, void *data);
extern void *llist_append(struct llist *llist, void *data);
extern void *llist_head(struct llist *llist);
extern void *llist_tail(struct llist *llist);
extern void *llist_find(struct llist *llist, void *data, int (*cmpfn)(void *, void *));
extern void *llist_delete(struct llist *llist, void *data, int (*cmpfn)(void *, void *));
extern int llist_count(struct llist *llist);
extern void llist_foreach(struct llist *llist, void (*f)(void *, void *), void *arg);
extern void **llist_array_get(struct llist *llist);
extern void llist_array_free(void **a);

#endif

