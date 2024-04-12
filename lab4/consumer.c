#include "header.h"

int main(int argc, char* argv[], char* envp[]){
    key_t key = 0;
    int flag  = 0;
    int msqid = 0;
    struct message msg;
    struct msqid_ds buf; 

    key = ftok(FTOK_1,FTOK_2);
    msqid = msgget(key, (IPC_CREAT | 0666 | IPC_NOWAIT));
    queue_stat(msqid);
    printf(LINE_SEPARATOR);

    //    do{
    //1. take mutex 

    //2. wait for msg in queue ?

    //3. TAKE msg
    flag = msgrcv(msqid, &msg, sizeof(msg), 0, IPC_NOWAIT); if(flag==-1){ strerror(errno); exit(-1); }

    //4. inc space

    //5. give mutex

    //6. PARSE msg
    (void)msgprint(msg);
    free(msg.content);

    flag = msgrcv(msqid, &msg, sizeof(msg), 0, IPC_NOWAIT); if(flag==-1){ strerror(errno); exit(-1); }
    (void)msgprint(msg);
    free(msg.content);


    flag = msgctl(msqid, IPC_RMID, &buf);
    if(flag==-1){
        strerror(errno);
        exit(-1);
    }

    //7. if signal, exit(1)
    //    }while(killed == 0);
    exit(1);
}
