#ifndef _NETFOO_H_
#define _NETFOO_H_

void *get_in_addr(struct sockaddr *sa);
int get_listener_socket(char *port);

#endif