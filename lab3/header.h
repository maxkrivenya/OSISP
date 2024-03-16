#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#define CHILD_PATH "./child"
#define CHILD_NAME "C"
#define CHILD_NAME_SIZE 3

int nanosleep(const struct timespec *req, struct timespec *rem);
int kill(pid_t pid, int sig);

struct combination{
    int zero;
    int one;
    int ten;
    int eleven;
};


volatile int print_allowed = -1;

void sig1_handler(int sig){
    signal(SIGUSR1, sig1_handler);
    print_allowed = 1;
}

void sig2_handler(int sig){
    signal(SIGUSR2, sig2_handler);
    print_allowed = 0;
}
