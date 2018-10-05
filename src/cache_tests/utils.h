#ifndef _UTILS_H_
#define _UTILS_H_

#include "../cache.h"

int check_strings(char *input, char *expected)
{
  for (; *input == *expected; input++, expected++) {
    if (*input == '\0') {
      return 0;
    }
  }
    
  return *input - *expected;
}

int check_cache_entries(struct cache_entry *input, struct cache_entry *expected)
{ 
  if (input == NULL) {
    return 1;
  }
  
  if (check_strings(input->path, expected->path) != 0) {
    return 1;
  }
  
  if (check_strings(input->content_type, expected->content_type) != 0) {
    return 1;
  }

  if (check_strings(input->content, expected->content) != 0) {
    return 1;
  }

  if (input->content_length != expected->content_length) {
    return 1;
  }

  return 0;
}

#endif