SRC=$(wildcard *.c)
HEADERS=$(wildcard *.h)
DEPS=$(SRC)$(HEADERS)

client: $(DEPS)
    gcc -Wall -Wextra -g -o $@ $(SRC)

.PHONY: clean

clean:
    rm -rf *.dSYM