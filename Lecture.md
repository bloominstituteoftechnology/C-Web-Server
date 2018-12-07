p      r    q
v      v    v
Hello, Beej World!\0

s
v
Beej \0

Goal:
Hello, Beej World!\0


char p[128] = "Hello, world!";

char s[128] = "Beej ";
int len = strlen(s);

char *q = r + len;

memmove(q, r, strlen(r) + 1); // just like memcpy, works for overlapping memory

memcpy(r, s, strlen(s));

puts(p);  // Hello, Beej World!