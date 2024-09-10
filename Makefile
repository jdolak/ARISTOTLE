CC = gcc

CFLAGS = -g -std=gnu99 -Wall -Iinclude -lncurses -pthread

all: aristotle

aristotle: src/*.c
	@$(CC) -o $@ $^ $(CFLAGS)

clean:
	-rm aristotle
	-rm ./*.o
	-rm -r ./*.dSYM

