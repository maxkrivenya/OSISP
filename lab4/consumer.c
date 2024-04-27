#include "header.h"

int main(int argc, char* argv[], char* envp[]){

    printf("consumer started\n");
    printf(LINE_SEPARATOR);

    key_t key = 0;
    int flag  = 0;
    int msqid = 0;
    int semid = 0;
    int current = 0;
    struct msqid_ds buf; 

    sem_t* mutex = sem_open(MUTEX_NAME,0);
    if(mutex==NULL){
        strerror(errno);
        exit(-1);
    }

    union semun arg;
    struct semid_ds semid_ds;
    arg.buf = &semid_ds;

    key = ftok(FTOK_1,FTOK_2);

    msqid = msgget(key, 0666);
    semid = semget(key, 2, 0666);

    if(semid == -1){
        strerror(errno);
        exit(-1);
    }

    do{

        //1. take mutex 
        sem_wait(mutex);


        //get semaphores
        flag = semctl(semid, 0 ,GETALL, arg.array);
        if(flag == -1){
            strerror(errno);
            exit(-1);
        }

        current = arg.array[1];

        //2. if there is msg in msgq
        if(arg.array[0] > arg.array[1]){

            //3. TAKE msg
            struct message msg;

            flag = msgrcv(msqid, &msg, sizeof(msg), 1, IPC_NOWAIT);
            if (flag==-1 || errno==ENOMSG){
                strerror(errno);
                exit(-1);
            }

            //4. inc received
            flag = semctl(semid, 1 ,SETVAL, current + 1);
            if (flag==-1 || errno==ENOMSG){
                strerror(errno);
                exit(-1);
            }

            //return mutex
            sem_post(mutex);

            //6. PARSE msg
            printf(LINE_SEPARATOR);
            printf("received message:\n");
            msgprint(msg);
            printf("updated received:%d\n", current + 1);
            printf(LINE_SEPARATOR);


        }
        else{
            //5. give mutex
            sem_post(mutex);
        }
        sleep(1);
    }while(!killed);

    printf("consumer exit\n");
    printf(LINE_SEPARATOR);

    exit(1);

    sem_close(mutex);
}
