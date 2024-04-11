#include "header.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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
    key_t key  = 11;

    struct msqid_ds buf; 

    int cnt = 0;
    char byte;
    unsigned short word;
    byte = rand()%257;
    struct message msg;
    msg.content = (char*)calloc(4 + byte + 1,1);
    if(msg.content==NULL){
        strerror(errno);
        exit(-1);
    }
    word = 1 + 13*byte;
    word = word/10;
    printf("\ttype:1\n\tword:%d\n\tsize:%d\n",word,byte);
    for(int i = 4; i < 4 + byte - 1 && i < 100; i++){
        msg.content[i] = rand()%27 + 65;
        word = word + msg.content[i] - 50;
    }
    msg.content[4+byte] = '\0';
    msg.mtype = 7;
    msg.content[0] = 7;
    msg.content[1] = ((unsigned char*)(&word))[0];
    msg.content[2] = ((unsigned char*)(&word))[1];
    msg.content[3] = byte;

    msgprint(msg);

    msqid = msgget(key, (IPC_CREAT | 0644));
    printf("\tmsqid      : %d\n",msqid);

    queue_stat(msqid); 


    msgsnd(msqid, &msg, sizeof(msg), 1);
    msgsnd(msqid, &msg, sizeof(msg), 1);

    queue_stat(msqid);

    free(msg.content);
    struct message rcv;
    printf("printing from msgrcv:\n");
    int i = 0;
    while(msgrcv(msqid,&rcv,sizeof(rcv),7,IPC_NOWAIT) != -1 && i < 10){
        msgprint(rcv);
        i = i + 1;
    }
    
       flag = msgctl(msqid, IPC_RMID, &buf);
       if(flag==-1){
       strerror(errno);
       exit(-1);
       }

       printf("removed msqid %d \n", msqid);
       
    printf("main exit\n");
    exit(1);
    return 0;
}
