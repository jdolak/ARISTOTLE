CC = gcc

CFLAGS = -g -std=gnu99 -Wall -Iinclude -lncurses -pthread -I/usr/include/libxml2 -lxml2

all: aristotle

aristotle: src/*.c
	@$(CC) -o $@ $^ $(CFLAGS)

check: aristotle
	valgrind --leak-check=full ./aristotle --local

clean:
	-rm aristotle
	-rm ./*.o
	-rm -r ./*.dSYM


