OBJS=server.o net.o file.o mime.o cache.o hashtable.o llist.o

server: $(OBJS)
	gcc -Wall -Wextra -o $@ $^

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
