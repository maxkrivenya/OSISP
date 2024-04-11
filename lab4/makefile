CC=gcc
CFLAGS= -W -Wall -Wextra -std=c11
.PHONY: clean

all: main producer
main: main.c makefile
		$(CC) $(CFLAGS) main.c -o main
producecr: producer.c makefile
		$(CC) $(CFLAGS) producer.c -o producer
