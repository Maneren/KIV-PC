CC ?= gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g3
BIN = graph.exe

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN): $(OBJS)
	$(CC) $^ -o $@

.PHONY: clean build execute valgrind
clean:
	rm $(BIN) $(OBJS)

build: $(BIN)

execute: build
	./$(BIN) $(ARGS)

valgrind: build
	valgrind ./$(BIN) $(ARGS) 2>&1 | tee valgrind.log
