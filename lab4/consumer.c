#include "header.h"

int main(int argc, char* argv[], char* envp[]){
    
    printf("consumer started\n");
    printf(LINE_SEPARATOR);

    key_t key = 0;
    int flag  = 0;
    int msqid = 0;
    int semid = 0;
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
    semid = semget(key, 2, 0644);

    if(semid == -1){
        strerror(errno);
        exit(-1);
    }

    //1. take mutex 
    printf("%s waiting for mutex...\n", argv[0]);
    sem_wait(mutex);
    printf("%s obtained mutex...\n", argv[0]);

printf("queue %d semaphres %d stats: %d-%d\n", msqid, semid, arg.array[0], arg.array[1]);

    //get semaphores
    flag = semctl(semid, 0 ,GETALL, arg.array);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }
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
        flag = semctl(semid, 1 ,SETVAL, arg.array[1] + 1);
        if (flag==-1 || errno==ENOMSG){
            strerror(errno);
            exit(-1);
        }
        printf("%d returned mutex\n", getpid());

        //return mutex
        sem_post(mutex);

        //6. PARSE msg
        (void)msgprint(msg);

    }
    else{
        //5. give mutex
        printf("%d returned mutex\n", getpid());
        sem_post(mutex);
    }

    printf("consumer exit\n");
    printf(LINE_SEPARATOR);

    exit(1);

    sem_close(mutex);
}
