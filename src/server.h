#ifndef _SERVER_H_
#define _SERVER_H_

struct arg_struct
{
    int arg1;
    struct cache *arg2;
};

extern pthread_mutex_t lock;

#endif