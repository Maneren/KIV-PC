CC ?= gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g3 -O3
BIN = kmemu.exe

DOC_FOLDER = doc
DOC = $(BIN:.exe=.pdf)
DOC_TARGET = $(DOC_FOLDER)/$(DOC)
DOC_SOURCE = $(DOC_TARGET:.pdf=.tex)

SRCS = $(wildcard src/*.c) $(wildcard src/**/*.c)
HEADERS = $(wildcard src/*.h) $(wildcard src/**/*.h)
OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN): $(OBJS)
	$(CC) $^ -o $@

.PHONY: clean build execute valgrind dist
clean:
	rm $(BIN) $(OBJS)

build: $(BIN)

execute: $(BIN)
	./$(BIN) $(ARGS)

valgrind: build
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(BIN) $(ARGS) 2>&1 | tee valgrind.log

$(DOC_TARGET):
	tectonic $(DOC_SOURCE) -o $(DOC_FOLDER) --keep-logs

dist: $(BIN) $(DOC_TARGET)
	rm -rf dist dist.zip
	git clone file://$(shell git rev-parse --show-toplevel) dist
	cp $(BIN) dist
	cp $(DOC_TARGET) dist
	cd dist && zip -r9 ../dist.zip Makefile $(HEADERS) $(SRCS) $(BIN) $(DOC)
