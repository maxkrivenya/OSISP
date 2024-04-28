#include "header.h"

volatile int PUSHED = 0;
volatile int PULLED = 0;

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

    key         = ftok(FTOK_1,FTOK_2);
    msqid       = msgget(key, 0666);

    do{
        //1. CREATE msg
        struct message msg = msg_create();

        //2. take mutex 
        sem_wait(mutex);

        current = PUSHED;

        //if enough space in q
        if(PUSHED - PULLED < MAX_MSG_AMT_IN_QUEUE){

            //3. PUT msg
            flag = msgsnd(msqid, &msg, sizeof(msg), 0);
            if(flag==-1){
                strerror(errno);
                exit(-1);
            }

            //4. increment pushed
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

    }while(!(*((int*)args)));

    *(int*)args = 0;

    printf("producer exit\n");
    printf(LINE_SEPARATOR);

    return NULL;
}

void* consumer(void* args){
    printf("consumer started\n");
    printf(LINE_SEPARATOR);

    key_t key = 0;
    int flag  = 0;
    int msqid = 0;
    int current = 0;

    sem_t* mutex = sem_open(MUTEX_NAME,0);
    if(mutex==NULL){
        strerror(errno);
        exit(-1);
    }

    key = ftok(FTOK_1,FTOK_2);

    msqid = msgget(key, 0666);

    do{

        //1. take mutex 
        sem_wait(mutex);

        current = PULLED;

        //2. if there is msg in msgq
        if(PUSHED > PULLED){

            //3. TAKE msg
            struct message msg;

            flag = msgrcv(msqid, &msg, sizeof(msg), 1, IPC_NOWAIT);
            if (flag==-1 || errno==ENOMSG){
                strerror(errno);
                exit(-1);
            }

            //4. inc received
            PULLED++;

            //5. return mutex
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

    }while(!(*((int*)args)));
    printf("consumer exit\n");
    printf(LINE_SEPARATOR);
    *(int*)args = 0;
    return NULL;
}
