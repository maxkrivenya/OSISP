#include "header.h"

int main(int argc, char* argv[], char* envp[]){
    printf(LINE_SEPARATOR);
    printf("consumer started\n");
    printf(LINE_SEPARATOR);
    
    key_t key = 0;
    int flag  = 0;
    int msqid = 0;
    int semid = 0;
    struct msqid_ds buf; 

    union semun arg;
    struct semid_ds semid_ds;
    arg.buf = &semid_ds;

    key = ftok(FTOK_1,FTOK_2);
    msqid = msgget(key, (IPC_CREAT | 0666 | IPC_NOWAIT));
    semid = semget(key, 4, IPC_CREAT | 0666);
    if(semid == -1){
        strerror(errno);
        exit(-1);
    }

//    do{
    for(int i = 0; i < 3; i++){
        //1. take mutex 
        flag = semctl(semid, 0 ,GETALL, arg.array);
        if(flag == -1){
            strerror(errno);
            exit(-1);
        }

        //2. if there is msg in msgq
        if(arg.array[1] > arg.array[2]){


            //3. TAKE msg
            struct message msg;

            flag = msgrcv(msqid, &msg, sizeof(msg), 1, IPC_NOWAIT);
            if (flag==-1 || errno==ENOMSG){
                break;
            }

            //4. inc received
            flag = semctl(semid, 2 ,SETVAL, arg.array[2] + 1);
            if (flag==-1 || errno==ENOMSG){
                break;
            }

            //5. give mutex
            flag = semctl(semid, 0, SETVAL, 1);
            if (flag==-1 || errno==ENOMSG){
                break;
            }

            //6. PARSE msg
            (void)msgprint(msg);
        }

    
    //7. if signal, exit(1)
    //}while(killed == 0);
}
    printf("consumer exit\n");
    printf(LINE_SEPARATOR);
    
    exit(1);
}
