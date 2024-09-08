CC = gcc

CFLAGS = -g -std=gnu99 -Wall -Iinclude -lncurses

all: aristotle

aristotle: src/main.c
	@$(CC) $(CFLAGS) -o $@ $<

clean:
	-rm aristotle
	-rm ./*.o
	-rm -r ./*.dSYM

