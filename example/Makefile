
CC = g++
CFLAGS = -Wall -Werror -Wextra -pedantic

all: main lib.so

main: main.cc
	$(CC) $(CFLAGS) -o $@ $^

lib.so: lib.cc
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^

.PHONY: clean
clean:
	rm -f main lib.so
