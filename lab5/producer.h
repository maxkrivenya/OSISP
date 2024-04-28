#include "header.h"

volatile int PUSHED = 0;
volatile int PULLED = 0;

//int main(int argc, char* argv[], char* envp[]){
void* producer(void* args){

    printf(LINE_SEPARATOR);
    printf("producer started\n");
    printf(LINE_SEPARATOR);

    key_t key       = 0;
    int flag        = 0;
    int msqid       = 0;
    int current     = 0;
    sem_t* mutex    = sem_open(MUTEX_NAME,0);

    if(mutex==NULL){
        strerror(errno);
        exit(-1);
    }

    //    union semun     arg;
    struct msqid_ds buf; 
    //    struct semid_ds semid_ds;
    //    arg.buf =       &semid_ds;

    key         = ftok(FTOK_1,FTOK_2);
    msqid       = msgget(key, 0666);
    //int semid   = semget(key, 2, 0666);


    do{
        //1. CREATE msg
        struct message msg = msg_create();

        //2. take mutex 
        sem_wait(mutex);

        /*
        //get semaphores
        flag = semctl(semid, 0 ,GETALL, arg.array);
        if(flag == -1){
        strerror(errno);
        exit(-1);
        }
        */
        //        current = arg.array[0];
        current = PUSHED;
        //if enough space in q
        //if(arg.array[0] - arg.array[1] < MAX_MSG_AMT_IN_QUEUE){
        if(PUSHED - PULLED < MAX_MSG_AMT_IN_QUEUE){

            //3. PUT msg
            flag = msgsnd(msqid, &msg, sizeof(msg), 0);
            if(flag==-1){
                strerror(errno);
                exit(-1);
            }

            /*
            //4. inc amt_sent
            flag = semctl(semid, 0 ,SETVAL, current + 1);
            if (flag==-1 || errno==ENOMSG){
            exit(1);
            }
            */
            PUSHED++;
            printf(LINE_SEPARATOR);
            printf("posted message:\n");
            msgprint(msg);
            printf("updated sent:%d\n", current + 1);
            printf(LINE_SEPARATOR);

        }

        //5. give mutex
        sem_post(mutex);
        sleep(1);

    }while(!producers_killed[*(int*)args]);

        producers_killed[*(int*)args] = 0;
        //sem_close(mutex);

        printf("producer exit\n");
        printf(LINE_SEPARATOR);

        return NULL;
        //exit(1);
    }

//int main(int argc, char* argv[], char* envp[]){
void* consumer(void* args){
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

    //union semun arg;
    //struct semid_ds semid_ds;
    //arg.buf = &semid_ds;

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

        /*
        //get semaphores
        flag = semctl(semid, 0 ,GETALL, arg.array);
        if(flag == -1){
        strerror(errno);
        exit(-1);
        }

        current = arg.array[1];
        */
        current = PULLED;

        //2. if there is msg in msgq
        //if(arg.array[0] > arg.array[1]){
        if(PUSHED > PULLED){
            //3. TAKE msg
            struct message msg;

            flag = msgrcv(msqid, &msg, sizeof(msg), 1, IPC_NOWAIT);
            if (flag==-1 || errno==ENOMSG){
                strerror(errno);
                exit(-1);
            }

            /*
            //4. inc received
            flag = semctl(semid, 1 ,SETVAL, current + 1);
            if (flag==-1 || errno==ENOMSG){
            strerror(errno);
            exit(-1);
            }
            */
            PULLED++;
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
    }while(!consumers_killed[*(int*)args]);
        printf("consumer exit\n");
        printf(LINE_SEPARATOR);
        consumers_killed[*(int*)args] = 0;
        //sem_close(mutex);
        return NULL;
    }
