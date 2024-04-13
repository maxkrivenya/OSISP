#include "header.h"

int main(int argc, char* argv[], char* envp[]){
    printf(LINE_SEPARATOR);
    printf("producer started\n");
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

   // do{
   for(int i = 0; i < 3; i++){  
   //1. CREATE msg
        struct message msg = msg_create();

        //2. take mutex 

        //get semaphores
        flag = semctl(semid, 0 ,GETALL, arg.array);
        if(flag == -1){
            strerror(errno);
            exit(-1);
        }

        //get msgq info
        flag = msgctl(msqid, IPC_STAT, &buf);
        if(flag == -1){
            strerror(errno);
            exit(-1);
        }

        //if not too many msg in msgq
        if(buf.msg_qnum > MAX_MSG_AMT_IN_QUEUE){

            //3. PUT msg
            flag = msgsnd(msqid, &msg, sizeof(msg), 0);
            if(flag==-1){
                strerror(errno);
                exit(-1);
            }

            //4. inc amt_sent
            flag = semctl(semid, 1 ,SETVAL, arg.array[1] + 1);
            if (flag==-1 || errno==ENOMSG){
                break;
            }

            //5. give mutex
            flag = semctl(semid, 0, SETVAL, 1);
            if (flag==-1 || errno==ENOMSG){
                break;
            }

        }

    //6. if signal, exit(1)
  //  }while(killed == 0);
}
    printf("producer exit\n");
    printf(LINE_SEPARATOR);

    exit(1);
}
