#include "header.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define n 10

//shared enum {false,true} choosing[n];
//shared int number[n];

//  choosing[i] = true;
//  number[i] = max(number[j=1->n]) + 1 (sup?)
//  choosing[i] = false
//  for(j in n){
//          while(choosing[i]);
//          while(number[j] && ((number[j],j) < (number[i],i));
//  }
//
//  DO STUFF
//
//  number[i] = 0
//

int main(int argc, char* argv[], char* envp[]){
    printf("main started\n");

    int msqid  = 0; 
    int flag   = 0; 
    key_t key  = 0;

    struct msqid_ds buf; 

    struct message msg = msg_create();
    struct message msg2 = msg_create();

    printf(LINE_SEPARATOR);
    msgprint(msg);
    printf(LINE_SEPARATOR);
    msgprint(msg2);
    printf(LINE_SEPARATOR);

    key = ftok(FTOK_1, FTOK_2);
    msqid = msgget(key, (IPC_CREAT | 0666));
    printf("\tmsqid      : %d\n",msqid);

    flag = msgsnd(msqid, &msg, sizeof(msg), 0);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

    printf(LINE_SEPARATOR);



    queue_stat(msqid);
    printf(LINE_SEPARATOR);

    flag = msgsnd(msqid, &msg2, sizeof(msg2), 0);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

    queue_stat(msqid);
    printf(LINE_SEPARATOR);
/*
    struct message rcv;
    printf("\nprinting from msgrcv:\n");
    int i = 0;
    while(msgrcv(msqid,&rcv,sizeof(rcv),0,IPC_NOWAIT) != -1 && i < 10){
        printf(LINE_SEPARATOR);
        msgprint(rcv);
        i = i + 1;
    }

    printf(LINE_SEPARATOR);

*/
    pid_t pid = fork();
    switch(pid){
        case -1:{
                    strerror(errno);
                    exit(-1);
                    break;
                }
        case 0:{
                   argv[0] = "./consumer";
                   argv[1] = NULL;
                    execve("./consumer", argv, envp);
                    strerror(errno);
                    exit(-1);
                    break;
               }

        default:{
                    break;
                }

    }
/*
    flag = msgctl(msqid, IPC_RMID, &buf);
    if(flag==-1){
        strerror(errno);
        exit(-1);
    }

    printf("removed msqid %d \n", msqid);
*/
    printf("main exit\n");

    free(msg.content);
    free(msg2.content);

    exit(1);
    return 0;
}
