# Doubly-Linked List

A doubly-linked list is very much like a standard linked list, except the links
go both to `next` and `prev`ious nodes in the list.

In addition to the pointer to the `head` of the list, we also keep a pointer to
the `tail` (last element) of the list.

This has a number of benefits:

* Traverse the list in either direction.

* If you already have a pointer to any element in this list, you can remove that
  element in O(1) time.
  
  (With a standard linked list, you need to traverse to keep track of the
  pointer to the previous element, making that an O(n) process.)

Drawbacks:

* There's more pointer manipulation when changing the list (to manage the `prev`
  pointers in addition to the `next` pointers).

## Use in an LRU cache

When used with an LRU cache, pointers to cache entries in the list are also kept
in a hash table.

This means if you want to move a just-used cache entry to the front of the list,
you can perform the following steps:

1. Look up the entry in the hash table: O(1)
2. Remove the entry from its position in the doubly linked list: O(1)
3. Insert the entry at the head of the doubly linked list: O(1)
