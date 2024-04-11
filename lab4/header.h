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
    printf("\ttype:%c\n\tword:%d\n\tsize:%c\n",msg.content[0], msg.content[1] + msg.content[2], msg.content[3]);
    for(int i = 0; i < msg.content[3]; i++){
        printf("%c", msg.content[i]);
    }
    printf("\n");
}
