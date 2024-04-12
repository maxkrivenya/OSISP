CC=gcc
CFLAGS= -W -Wall -Wextra -std=c11
.PHONY: clean

all: main producer consumer
main: main.c makefile
		$(CC) $(CFLAGS) main.c -o main
producer: producer.c makefile
		$(CC) $(CFLAGS) producer.c -o producer
consumer: consumer.c makefile
		$(CC) $(CFLAGS) consumer.c -o consumer
