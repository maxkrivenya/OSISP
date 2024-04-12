#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <errno.h>
#include <error.h>
#include <string.h>

#include <signal.h>

#define LINE_SEPARATOR "\n------------------------------------------------\n"
#define FTOK_1 "main"
#define FTOK_2 65
#define MAX_MSG_AMT_IN_QUEUE 3

union semun {
    int val; // значение для SETVAL
    struct semid_ds *buf; // буфер для IPC_STAT, IPC_SET
    unsigned short *array; // массив для GETALL, SETALL
    struct seminfo *__buf; // буфер для IPC_INFO (есть только в Linux) */
} ;
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
