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

volatile int killed = 0;
void sig1_handler(int sig){
    signal(SIGUSR1, sig1_handler);
    killed = 1;
}

volatile int msg_read = 0;
void sig2_handler(int sig){
    signal(SIGUSR2, sig2_handler);
    msg_read = 1;
}
struct message{
    long mtype;
    //char* content;
    char content[260];
};

/*
 * content[0]                           = type
 * content[1->2]                        = hash
 * content[3]                           = size
 * content[4 -> (4 + ((size+3)/4)*4)]   = data
 *
 * */

void msq_stat(int msqid){
    int flag = 0;
    struct msqid_ds buf; 
   
    flag = msgctl(msqid, IPC_STAT, &buf);
    if(flag==-1){
        strerror(errno);
        exit(-1);
    }

    printf("\tamt of msgs: %lu\n", buf.msg_qnum);
   
}

void msgprint(struct message msg){
    int size = msg.content[3] & 0xFF;
    printf("\ttype:%c\n\thash:%c%c\n\tsize:%d\n\tdata:",msg.content[0], msg.content[1] , msg.content[2], size);
    for(int i = 0; i < size; i++){
        printf("%c", msg.content[i]);
    }
    printf(LINE_SEPARATOR);
}

struct message msg_create(){
    struct message msg;
    int sizeint = 0;
    char sizechar = '0';
    unsigned short word = 0;
    do{
        sizeint = rand()%257;
    }while(!sizeint);

    if(sizeint < 256){
        sizechar = sizeint;
    }
    msg.mtype = 1;
//    msg.content = (char*)calloc(4 + sizeint + 1, 1);
    for(int i = 4; i < 4 + sizeint; i++){
        msg.content[i] = 'A' + rand()%26;
        word = word + (msg.content[i] % 10);
    }
    msg.content[4 + sizeint] = '\0';

    msg.content[0] = '1';
    msg.content[1] = ((unsigned char*)(&word))[0];
    msg.content[2] = ((unsigned char*)(&word))[1];
    msg.content[3] = sizechar;

    return msg;
}
