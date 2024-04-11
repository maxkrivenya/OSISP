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

#define LINE_SEPARATOR "\n------------------------------------------------\n"

struct message{
    long mtype;
    char* content;
};

/*
 * content[0]                           = type
 * content[1->2]                        = hash
 * content[3]                           = size
 * content[4 -> (4 + ((size+3)/4)*4)]   = data
 *
 * */

void queue_stat(int msqid){
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
    if(msg.content == NULL){return;}
    printf("\ttype:%c\n\thash:%d\n\tsize:%c\n\tdata:",msg.content[0], msg.content[1] + msg.content[2], msg.content[3]);
    for(int i = 0; i < msg.content[3]; i++){
        printf("%c", msg.content[i]);
    }
    printf("size char: %d\n", 0 + msg.content[3]); 
    printf("\n");
}

struct message msg_create(){
    struct message msg;
    int sizeint = 0;
    char sizechar = '0';
    unsigned short word = 0;
    do{
        sizeint = rand()%257;
    }while(!sizeint);

    printf(LINE_SEPARATOR);
    printf("size:%d\n", sizeint);
    printf(LINE_SEPARATOR);
    
    if(sizeint < 256){
        sizechar = sizeint;
    }
    msg.mtype = 1;
    msg.content = (char*)calloc(4 + sizeint + 1, 1);
    for(int i = 4; i < 4 + sizeint; i++){
        msg.content[i] = 'A' + rand()%27;
        word = word + (msg.content[i] % 10);
    }
    msg.content[0] = '1';
    msg.content[1] = ((unsigned char*)(&word))[0];
    msg.content[2] = ((unsigned char*)(&word))[1];
    msg.content[3] = sizechar;

    return msg;
}
