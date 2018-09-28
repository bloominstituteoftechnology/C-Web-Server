#include <stdlib.h>
#include "llist.h"

struct llist_node {
	void *data;
	struct llist_node *next;
};

/**
 * Allocate a new linked list
 */
struct llist *llist_create(void)
{
	return calloc(1, sizeof(struct llist));
}

/**
 * Destroy a linked list
 *
 * If destfn is not NULL, it is called with the node data before the
 * node is deallocated.
 *
 * NOTE: does *not* deallocate the data in each node!!
 */
void llist_destroy(struct llist *llist)
{
	struct llist_node *n = llist->head, *next;

	while (n != NULL) {
		next = n->next;
		free(n);

		n = next;
	}

	free(llist);
}

/**
 * Insert at the head of a linked list
 */
void *llist_insert(struct llist *llist, void *data)
{
	struct llist_node *n = calloc(1, sizeof *n);

	if (n == NULL) {
		return NULL;
	}

	n->data = data;
	n->next = llist->head;
	llist->head = n;

	llist->count++;

	return data;
}

/**
 * Append to the end of a list
 */
void *llist_append(struct llist *llist, void *data)
{
	struct llist_node *tail = llist->head;

	// If list is empty, just insert
	if (tail == NULL) {
		return llist_insert(llist, data);
	}

	struct llist_node *n = calloc(1, sizeof *n);

	if (n == NULL) {
		return NULL;
	}

	while (tail->next != NULL) {
		tail = tail->next;
	}

	n->data = data;
	tail->next = n;

	llist->count++;

	return data;
}

/**
 * Return the first element in a list
 */
void *llist_head(struct llist *llist)
{
	if (llist->head == NULL) {
		return NULL;
	}

	return llist->head->data;
}

/**
 * Return the last element in a list
 */
void *llist_tail(struct llist *llist)
{
	struct llist_node *n = llist->head;

	if (n == NULL) {
		return NULL;
	}

	while (n->next != NULL) {
		n = n->next;
	}

	return n->data;
}

/**
 * Find an element in the list
 *
 * cmpfn should return 0 if the comparison to this node's data is equal.
 */
void *llist_find(struct llist *llist, void *data, int (*cmpfn)(void *, void *))
{
	struct llist_node *n = llist->head;

	if (n == NULL) {
		return NULL;
	}

	while (n != NULL) {
		if (cmpfn(data, n->data) == 0) {
			break;
		}

		n = n->next;
	}

	if (n == NULL) {
		return NULL;
	}

	return n->data;
}

/**
 * Delete an element in the list
 *
 * cmpfn should return 0 if the comparison to this node's data is equal.
 *
 * NOTE: does *not* free the data--it merely returns a pointer to it
 */
void *llist_delete(struct llist *llist, void *data, int (*cmpfn)(void *, void *))
{
	struct llist_node *n = llist->head, *prev = NULL;

	while (n != NULL) {
		if (cmpfn(data, n->data) == 0) {

			void *data = n->data;

			if (prev == NULL) {
				// Free the head
				llist->head = n->next;
				free(n);

			} else {
				// Free the non-head
				prev->next = n->next;
				free(n);
			}

			llist->count--;

			return data;
		}

		prev = n;
		n = n->next;
	}

	return NULL;
}

/**
 * Return the number of elements in the list
 */
int llist_count(struct llist *llist)
{
	return llist->count;
}

/**
 * For each item in the list run a function
 */
void llist_foreach(struct llist *llist, void (*f)(void *, void *), void *arg)
{
	struct llist_node *p = llist->head, *next;

	while (p != NULL) {
		next = p->next;
		f(p->data, arg);
		p = next;
	}
}

/**
 * Allocates and returns a new NULL-terminated array of pointers to data
 * elements in the list.
 *
 * NOTE: This is a read-only array! Consider it an array view onto the linked
 * list.
 */
void **llist_array_get(struct llist *llist)
{
	if (llist->head == NULL) {
		return NULL;
	}

	void **a = malloc(sizeof *a * llist->count + 1);

	struct llist_node *n;
	int i;

	for (i = 0, n = llist->head; n != NULL; i++, n = n->next) {
		a[i] = n->data;
	}

	a[i] = NULL;

	return a;
}

/**
 * Free an array allocated with llist_array_get().
 * 
 * NOTE: this does not modify the linked list or its data in any way.
 */
void llist_array_free(void **a)
{
	free(a);
}
