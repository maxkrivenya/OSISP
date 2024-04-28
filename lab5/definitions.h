#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <errno.h>
#include <error.h>
#include <string.h>

#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>

#include <pthread.h>


#define MUTEX_NAME "my little mutex"
#define LINE_SEPARATOR "\n------------------------------------------------\n"
#define FTOK_1 "main"
#define FTOK_2 65
#define MAX_CHILD_AMT 5

struct message{
    long mtype;
    char content[260];
};

/*
 * content[0]                           = type
 * content[1->2]                        = hash
 * content[3]                           = size
 * content[4 -> (4 + ((size+3)/4)*4)]   = data
 *
 * */
