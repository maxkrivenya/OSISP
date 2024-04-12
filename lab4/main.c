#include "header.h"
#define n 5

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
    printf(LINE_SEPARATOR);
    printf("main started\n");
    printf(LINE_SEPARATOR);

    int semid  = 0;
    int msqid  = 0; 
    int flag   = 0; 
    key_t key  = 0;

    struct msqid_ds buf; 
    
    union semun arg;
    struct semid_ds semid_ds;
    arg.buf = &semid_ds;

    struct message msg = msg_create();
    struct message msg2 = msg_create();
    struct message msg3 = msg_create();


    key = ftok(FTOK_1, FTOK_2);

    //int semget(key_t key, int nsems, int semflg)
    semid = semget(key, 4, IPC_CREAT | 0666);
    if(semid == -1){
        strerror(errno);
        exit(-1);
    }

    flag = semctl(semid, 0, GETALL, arg.array);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }
    if(arg.array != NULL){
        for (int i = 0; i < 4; i++){ 
            printf("\t%d:%d\n", i, arg.array[i]);
        }
    }


    printf(LINE_SEPARATOR);

    arg.val = 1;
    flag = semctl(semid, 1, SETVAL, 5);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

    arg.buf = &semid_ds;
    flag = semctl(semid, 0 ,GETALL, arg.array);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

    if(arg.array != NULL){
        for (int i = 0; i < 4; i++){ 
            printf("\t%d:%d\n", i, arg.array[i]);
        }
    }
    
    msqid = msgget(key, (IPC_CREAT | 0666));
    if(msqid == -1){
        strerror(errno);
        exit(-1);
    }

    flag = msgsnd(msqid, &msg, sizeof(msg), 0);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

    printf(LINE_SEPARATOR);

    flag = msgsnd(msqid, &msg2, sizeof(msg2), 0);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

    flag = msgsnd(msqid, &msg3, sizeof(msg3), 0);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

    pid_t pid = fork();
    switch(pid){
        case -1:{
                    strerror(errno);
                    exit(-1);
                    break;
                }
        case 0:{
                   argv[0] = "./consumer";
                   argv[1] = NULL;
                   execve("./consumer", argv, envp);
                   strerror(errno);
                   exit(-1);
                   break;
               }

        default:{
                    break;
                }

    }
    flag = msgctl(msqid, IPC_RMID, &buf);
    if(flag==-1){
        strerror(errno);
        exit(-1);
    }

    printf("main exit\n");
    printf(LINE_SEPARATOR);

    exit(1);
    return 0;
}
