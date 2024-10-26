CC ?= gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g3 -O3
BIN = kmemu.exe

SRCS = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJS = $(SRCS:.c=.o)

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
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(BIN) $(ARGS) 2>&1 | tee valgrind.log
