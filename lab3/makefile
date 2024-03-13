CC=gcc
CFLAGS= -W -Wall -Wextra -std=c11
.PHONY: clean

all: main child
main: main.c makefile
		$(CC) $(CFLAGS) main.c -o main
child: child.c makefile
		$(CC) $(CFLAGS) child.c -o child
