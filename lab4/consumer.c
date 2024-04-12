#include "header.h"
#include <signal.h>

int main(int argc, char* argv[], char* envp[]){
    printf(LINE_SEPARATOR);
    printf("consumer started\n");
    printf(LINE_SEPARATOR);
    
    key_t key = 0;
    int flag  = 0;
    int msqid = 0;
    struct msqid_ds buf; 

    key = ftok(FTOK_1,FTOK_2);
    msqid = msgget(key, (IPC_CREAT | 0666 | IPC_NOWAIT));
    //msq_stat(msqid);

    //    do{
    //1. take mutex 

    //2. wait for msg in queue ?

    //3. TAKE msg
    while(1){
        struct message msg;
        flag = msgrcv(msqid, &msg, sizeof(msg), 1, IPC_NOWAIT);
        if (flag==-1 || errno==ENOMSG){
            break;
        }
        //4. inc space

        //5. give mutex

        //6. PARSE msg

        (void)msgprint(msg);
    }
    flag = msgctl(msqid, IPC_RMID, &buf);
    if(flag==-1){
        strerror(errno);
        exit(-1);
    }
    //7. if signal, exit(1)
    //    }while(killed == 0);
    kill(getppid(), SIGUSR2);
    printf("consumer exit\n");
    printf(LINE_SEPARATOR);
    exit(1);
}
