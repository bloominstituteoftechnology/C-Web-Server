CFLAGS=-std=c99 -Wall -Wextra

OBJS=server.o net.o file.o mime.o cache.o hashtable.o llist.o

server: $(OBJS)
	gcc -Wall -Wextra -g -o $@ $^

net.o: net.c net.h
	gcc -Wall -Wextra -c $<

server.o: server.c net.h
	gcc -Wall -Wextra -c $<

file.o: file.c file.h
	gcc -Wall -Wextra -c $<

mime.o: mime.c mime.h
	gcc -Wall -Wextra -c $<

cache.o: cache.c cache.h
	gcc -Wall -Wextra -c $<

hashtable.o: hashtable.c hashtable.h
	gcc -Wall -Wextra -c $<

llist.o: llist.c llist.h
	gcc -Wall -Wextra -c $<

.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f server
	rm -f cache_tests/cache_tests
	rm -f cache_tests/cache_tests.exe
	rm -f cache_tests/cache_tests.log

TEST_SRC=$(wildcard cache_tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

cache_tests/cache_tests:
	cc cache_tests/cache_tests.c cache.c hashtable.c llist.c -o cache_tests/cache_tests

test:
	tests

tests: clean $(TESTS)
	sh ./cache_tests/runtests.sh

.PHONY: tests
