#ifndef _FILELS_H_ // This was just _FILE_H_, but that interfered with Cygwin
#define _FILELS_H_

struct file_data {
    int size;
    void *data;
};

extern struct file_data *file_load(char *filename);
extern void file_free(struct file_data *filedata);

#endif
