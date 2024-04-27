#include "header.h"
#include <string.h>

int main(int argc, char* argv[], char* envp[]){
    printf(LINE_SEPARATOR);
    printf("producer started\n");
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

    union semun arg;
    struct msqid_ds buf; 
    struct semid_ds semid_ds;
    arg.buf = &semid_ds;

    key         = ftok(FTOK_1,FTOK_2);
    msqid       = msgget(key, 0666);
    int semid   = semget(key, 2, 0666);

    //1. CREATE msg
    struct message msg = msg_create();

    //2. take mutex 
    
    //printf("%s waiting for mutex...\n", argv[0]);
    sem_wait(mutex);
    //printf("%d obtained mutex\n", getpid());


    //get semaphores
    flag = semctl(semid, 0 ,GETALL, arg.array);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

printf("queue %d semaphres %d stats: %d-%d\n", msqid, semid, arg.array[0], arg.array[1]);
    current = arg.array[0];

    /*
    //get msgq info
    arg.buf = &semid_ds;
    flag = msgctl(msqid, IPC_STAT, &buf);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }
*/
    //if(buf.msg_qnum < MAX_MSG_AMT_IN_QUEUE){
    if(arg.array[0] - arg.array[1] < 10){
        //3. PUT msg
        flag = msgsnd(msqid, &msg, sizeof(msg), 0);
        if(flag==-1){
            strerror(errno);
            exit(-1);
        }

        //4. inc amt_sent
        flag = semctl(semid, 0 ,SETVAL, current + 1);
        if (flag==-1 || errno==ENOMSG){
            exit(1);
        }

        printf(LINE_SEPARATOR);
        printf("posted message:\n");
        msgprint(msg);
        printf("updated sent:%d\n", current + 1);
        printf(LINE_SEPARATOR);

    }
    //5. give mutex
    //printf("%s returned mutex\n", argv[0]);
    sem_post(mutex);
    sem_close(mutex);

    printf("producer exit\n");
    printf(LINE_SEPARATOR);

    exit(1);
}
