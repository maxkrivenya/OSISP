#include "header.h"
#define n 5

//shared enum {false,true} choosing[n];
//shared int number[n];

//  choosing[i] = true;
//  sem_close(mutex)
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

    sem_unlink(MUTEX_NAME);
    int semid  = 0;
    int msqid  = 0; 
    int flag   = 0; 
    key_t key  = 0;

    sem_t* mutex = sem_open(MUTEX_NAME , O_CREAT, 0666, 1);
    
    struct msqid_ds buf; 
    
    union semun arg;
    struct semid_ds semid_ds;
    arg.buf = &semid_ds;

    key = ftok(FTOK_1, FTOK_2);

    //int semget(key_t key, int nsems, int semflg)
    semid = semget(key, 2, IPC_CREAT | 0666);
    if(semid == -1){
        strerror(errno);
        exit(-1);
    }

    flag = semctl(semid, 0, GETALL, arg.array);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }
    flag = semctl(semid, 0, SETVAL, 0);
    flag = semctl(semid, 1, SETVAL, 0);
    flag = semctl(semid, 0, GETALL, arg.array);
    if(flag == -1){
        strerror(errno);
        exit(-1);
    }

    if(arg.array != NULL){
        for (int i = 0; i < 2.; i++){ 
            printf("\t%d:%d\n", i, arg.array[i]);
        }
    }

    printf(LINE_SEPARATOR);

    msqid = msgget(key, (IPC_CREAT | 0666));
    if(msqid == -1){
        strerror(errno);
        exit(-1);
    }
pid_t pid = 0;
    int i       = 0;
    int id      = 0;
    int producer_counter = 0;
    int consumer_counter = 0;
    pid_t max_producer_pid   = 0;
    pid_t max_consumer_pid   = 0;
    pid_t producers[MAX_CHILD_AMT];
    pid_t consumers[MAX_CHILD_AMT];
    for(i = 0; i < MAX_CHILD_AMT; i++){
        producers[i] = 0;
        consumers[i] = 0;
    }

    char comm   = '\n';   // command (+/-/g/s/p)
    char* get   = (char*)calloc(10,1); //init g<K> string
    char* name  = (char*)calloc(10,1); //init "CHILD%d%d" string
                                                    
    if(get==NULL){
        printf("%s\n",strerror(errno));
        exit(-1);
    } 
    if(name==NULL){
        printf("%s\n",strerror(errno));
        exit(-1);
    }

    (void)strcpy(name,  "C<K>");
    if(name==NULL){
        printf("%s\n",strerror(errno));
        exit(-1);
    }

    (void)fflush(stdin);
    do{
        if(producer_counter < 0){
            for(producer_counter = 0; 
                    producer_counter < MAX_CHILD_AMT && producers[producer_counter] != 0; 
                    producer_counter++
               );  //counter = FIRST i WHERE child[i] = 0
        }
        if(consumer_counter < 0){
            for(consumer_counter = 0; 
                    consumer_counter < MAX_CHILD_AMT && consumers[consumer_counter] != 0; 
                    consumer_counter++
               );  //counter = FIRST i WHERE child[i] = 0
        }

        printf(": ");

        get = fgets(get, 10, stdin); //get  = "CX<Y>"
        comm = get[1];                             //comm = 'X'
        if(get[2] == '<' && isdigit(get[3])){      //if "X<",
            id = get[3] - '0';                       //get number
        }else{
            id = -1;
        }

        switch(comm){
            case '\n':{
                          break;
                      }
            case 'q':{
                         (void)free(name);
                         sem_unlink(MUTEX_NAME);

                         printf("main exit\n");
                         flag = kill(0,SIGINT);
                         printf(LINE_SEPARATOR);

                         exit(1);
                         return 0;

                         break;
                     }
            case 'k':{
                         if(id==-1){
                             (void)puts("killing everyone...\n");
                             for(int i = 0; i < MAX_CHILD_AMT; i++){
                                 if(producers[i] != 0){
                                     flag = kill(producers[i],SIGINT);
                                     if(flag==-1){
                                         printf("%s\n",strerror(errno));
                                         exit(-1);
                                     }
                                     producers[i] = 0;
                                 }
                                 if(consumers[i] != 0){
                                     flag = kill(consumers[i],SIGINT);
                                     if(flag==-1){
                                         printf("%s\n",strerror(errno));
                                         exit(-1);
                                     }
                                 }
                             }
                             (void)puts("everyone is dead.\n");
                         }else{
                             if(id >= 0 && id < MAX_CHILD_AMT){
                                 if(get[0]=='p'){
                                     if(producers[id] != 0){
                                         flag = kill(producers[id],SIGINT);
                                         if(flag==-1){
                                             printf("%s\n",strerror(errno));
                                             exit(-1);
                                         }
                                         printf("killed P%d - %d\n",id,producers[id]);
                                         producers[id]=0;
                                     }else{
                                         printf("no such process!\n");
                                     }
                                 }
                                 else{
                                     if(consumers[id] != 0){
                                         flag = kill(consumers[id],SIGINT);
                                         if(flag==-1){
                                             printf("%s\n",strerror(errno));
                                             exit(-1);
                                         }
                                         printf("killed C%d - %d\n",id,consumers[id]);
                                         consumers[id]=0;
                                     }else{
                                         printf("no such process!\n");
                                     }
                                 }
                             }else{
                                 printf("id out of range 0-%d\n",MAX_CHILD_AMT);
                             }
                         }
                         if(get[0]=='p'){
                             producer_counter = -1;
                         }else{
                             consumer_counter = -1;
                         }
                         break;
                     } 
            case 'l':{  
                         for (int i = 0; i < MAX_CHILD_AMT; i++) {

                             if(producers[i] != 0){ 
                                 printf("\tP%d-'%d'\t",i,producers[i]);
                             }else{
                                 printf("\tP%d-'X'\t",i);
                             }

                             if(consumers[i] != 0){
                                 printf("\tC%d-'%d'\n",i,consumers[i]);
                             }else{
                                 printf("\tC%d-'X'\n",i);
                             }
                         }
                         break;
                     }
            case '+':{
                         if(id == -1){id = 1;}
                         for(i = 0; i < id; i++){
                             if(get[0]=='p'){
                                 if(producer_counter < 0){
                                     for(producer_counter = 0; 
                                             producer_counter < MAX_CHILD_AMT && producers[producer_counter] != 0; 
                                             producer_counter++
                                        );  //counter = FIRST i WHERE child[i] = 0
                                 }
                                 if(producer_counter < 0 || producer_counter >= MAX_CHILD_AMT){
                                     printf("\nERROR: can't make more children! max amt = %d\n\n", MAX_CHILD_AMT);
                                     producer_counter = -1;
                                     break;
                                 }

                                 //forking
                                 pid = fork();

                                 switch (pid) {
                                     case -1:{                           //fork failed
                                                 printf("%s\n",strerror(errno));
                                                 (void)free(name);
                                                 (void)exit(EXIT_FAILURE);

                                             }

                                     case 0:{                            //for forked

                                                //update process name
                                                // for(i = 0; i < MAX_CHILD_AMT && child[i] != 0 && child[i] != getpid(); i++);
                                                name[0]='p';
                                                name[2] = '0' + producer_counter;
                                                argv[0] = name;
                                                flag = execve("./producer", argv, envp);
                                                if(flag == -1){
                                                    (void)printf("execve error:%s\n", strerror(errno));
                                                    (void)exit(EXIT_FAILURE);
                                                }
                                                exit(1);
                                                break;
                                            }

                                     default:{       //for parent
                                                 printf("created P%d-%d\n",producer_counter,pid);
                                                 producers[producer_counter] = pid;
                                                 break;
                                             }      
                                 }
                                 producer_counter = -1;
                             }
                             else{
                                 if(consumer_counter < 0){
                                     for(consumer_counter = 0; 
                                             consumer_counter < MAX_CHILD_AMT && consumers[consumer_counter] != 0; 
                                             producer_counter++
                                        );  //counter = FIRST i WHERE child[i] = 0
                                 }
                                 if(consumer_counter < 0 || consumer_counter >= MAX_CHILD_AMT){
                                     printf("\nERROR: can't make more consumers! max amt = %d\n\n", MAX_CHILD_AMT);
                                     consumer_counter = -1;
                                     break;
                                 }

                                 //forking
                                 pid = fork();

                                 switch (pid) {
                                     case -1:{                           //fork failed
                                                 printf("%s\n",strerror(errno));
                                                 (void)free(name);
                                                 (void)exit(EXIT_FAILURE);

                                             }

                                     case 0:{                            //for forked

                                                //update process name
                                                // for(i = 0; i < MAX_CHILD_AMT && child[i] != 0 && child[i] != getpid(); i++);
                                                name[0]='c';
                                                name[2] = '0' + consumer_counter;
                                                argv[0] = name;
                                                flag = execve("./consumer", argv, envp);
                                                if(flag == -1){
                                                    (void)printf("execve error:%s\n", strerror(errno));
                                                    (void)exit(EXIT_FAILURE);
                                                }
                                                exit(1);
                                                break;
                                            }

                                     default:{       //for parent
                                                 printf("created C%d-%d\n",consumer_counter,pid);
                                                 consumers[consumer_counter] = pid;
                                                 break;
                                             }      
                                 }
                                 consumer_counter = -1;
                             }
                         }
                         break;
                     }
            case '-':{
                         if(id == -1){id = 1;}
                         for(int i = 0; i < id; i++){
                             pid_t max_pid = -1;
                             int max_pid_id = -1;
                             flag = 0;


                             for(int i = 0; i < MAX_CHILD_AMT; i++){
                                 //SELECT child WHERE MAX(pid)
                                 if(get[0]=='p'){
                                     if(producers[i] > max_pid && producers[i] != 0){                              
                                         max_pid = producers[i];
                                         max_pid_id = i;
                                     }
                                 }
                                 else{
                                     if(consumers[i] > max_pid && consumers[i] != 0){                              
                                         max_pid = consumers[i];
                                         max_pid_id = i;
                                     }
                                 }
                             }
                             
                             if(max_pid > 0 
                                     && max_pid_id < MAX_CHILD_AMT 
                                     && max_pid_id > 0
                               ){
                                 flag = kill(max_pid,SIGINT);
                                 if(flag==-1){
                                     printf("%s\n",strerror(errno));
                                     exit(-1);
                                 }
                                 if(get[0]=='p'){
                                     printf("killed P%d-%d\n",max_pid_id,max_pid);
                                     producers[max_pid_id] = 0;
                                 }else{
                                     printf("killed C%d-%d\n",max_pid_id,max_pid);
                                     consumers[max_pid_id] = 0;
                                 }
                             }else{
                                 
                                 if(get[0]=='p'){
                                     printf("no more producers to delete\n");
                                 }else{
                                     printf("no more consumers to delete\n");
                                 }
                                 break;
                             }
                             if(flag==-1){
                                 printf("%s\n",strerror(errno));
                                 printf("id = '%d', max_pid='%d'\n",max_pid_id,max_pid);
                                 exit(-1);
                             }
                         }
                         if(get[0]=='p'){
                             producer_counter = -1;
                         }else{
                             consumer_counter= -1;
                         }
                         break;
                     }
            default:{
                        printf("unknown command\n");
                        break;

                    }
        } 

    }while(1);

    exit(1);
}
