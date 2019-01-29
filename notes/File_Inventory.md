
cache.c - 

serverfiles/404.html - displays 404 message

serverroot/index.html - home page welcome message

file.c - loads a file into memory and returns a pointer to the data

hashtable.c - creates a hashtable struct, several helper functions include table entry, cleanup linked list, change entry count, default modulo hashing, free entry, destroy table, put string/binary key, comparison function, get from hashtable (string/binary key), delete from hashtable (string/binary key), etc.

llist.c - allocate new linked list, destroy linked list, insert at head, append list, return first element, return last element, find element, delete element, return size of list, run function on each item in list, read only get (array view of linked list), free array allocated with llist_array_get()

mime.c - string to lowercase, return type of MIME

net.c - get an internet address, return main listening socket, look for matching local network interfaces, loops through potentail interfaces and try to set up a socket in each, listen

server.c - main server code: send a response, get a random number function, sends a 404 response, get a file, find the start of body, handle an http request, main function



