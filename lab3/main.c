#include "header.h"
#include <signal.h>
#include <stdio.h>
#define COMMAND_LENGTH 6 
#define MAX_CHILD_AMT 10

int main(int argc, char *argv[], char *envp[]){
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        (void)perror("signal");
        (void)exit(EXIT_FAILURE);
    }

    int i       = 0;
    int id      = 0;
    int flag    = 0;
    int counter = 0;


    pid_t pid       = 0;
    pid_t max_pid   = 0;
    pid_t child[MAX_CHILD_AMT];
    for(i = 0; i < MAX_CHILD_AMT; i++){
        child[i] = 0;
    }

    char comm   = '\n';   // command (+/-/g/s/p)
    char* get   = (char*)calloc(COMMAND_LENGTH,1); //init g<K> string
    char* name  = (char*)calloc(NAME_SIZE,1); //init "CHILD%d%d" string
    if(get==NULL){
        perror("malloc");
        exit(-1);
    } 
    if(name==NULL){
        perror("malloc");
        exit(-1);
    }
    (void)strcpy(name,  CHILD_NAME);
    if(name==NULL){
        perror("strcpy");
        exit(-1);
    }

    (void)fflush(stdin);
    do{
        if(counter < 0){
            for(counter = 0; 
                counter < MAX_CHILD_AMT && child[counter] != 0; 
                counter++
            );  //counter = FIRST i WHERE child[i] = 0
        }
       printf(": ");
        get = fgets(get, COMMAND_LENGTH-1, stdin); //get  = "X<Y>"
        comm = get[0];                             //comm = 'X'
        if(get[1] == '<' && isdigit(get[2])){      //if "X<",
            id = get[2] - '0';                       //get number
        }else{
            id = -1;
        }
        switch(comm){
            case '\n':{
                          break;
                      }
            case 'q':{
                         (void)free(name);
                         (void)puts("parent exit");
                         flag = kill(0,SIGINT);
                         if(flag==-1){
                             perror("kill");
                             exit(-1);
                         }
                         (void)exit(1);
                         break;
                     }
            case 'k':{
                         if(id==-1){
                             (void)puts("killing everyone...\n");
                             for(int i = 0; i < MAX_CHILD_AMT; i++){
                                 if(child[i] != 0){
                                     flag = kill(child[i],SIGINT);
                                     child[i] = 0;
                                 }
                             }
                             (void)puts("everyone is dead.\n");
                         }else{
                             if(id > 0 && id < MAX_CHILD_AMT){
                                 if(child[id] != 0){
                                     kill(child[id],SIGINT);
                                     printf("killed child%d - %d\n",id,child[id]);
                                     child[id]=0;
                                 }else{
                                     printf("no such process!\n");
                                 }
                             }else{
                                 printf("id out of range 0-%d\n",MAX_CHILD_AMT);
                             }
                         }
                         counter = -1;
                         break;
                     }
            case 'g':{
                         signal(SIGUSR1,sig1_handler);
                         if(id < 0){
                             printf("everyone is allowed to print.\n");
                             for(i = 0; i < MAX_CHILD_AMT; i++){
                                 if(child[i] != 0){
                                     flag = kill(child[i],SIGUSR1);
                                 }
                             }
                         }else{
                             if(id < MAX_CHILD_AMT){
                                 if(child[id] != 0){
                                     printf("C%d-%d is allowed to print\n",i,child[i]);
                                     flag = kill(child[id], SIGUSR1);
                                 }
                             }else{
                                 printf("pid(C<%d>) = 0!\n", id);
                             }
                         }
                         if(flag==-1){
                             perror("kill");
                             exit(-1);
                         }
                         break;
                     }
            case 's':{
                         signal(SIGUSR2,sig2_handler);
                         if(id < 0){
                             printf("everyone is forbidden to print.\n");
                             for(i = 0; i < MAX_CHILD_AMT; i++){
                                 if(child[i] != 0){
                                     flag = kill(child[i],SIGUSR2);
                                 }
                             }
                         }else{
                             if(id < MAX_CHILD_AMT){
                                 if(child[id] != 0){
                                     printf("C%d-%d is forbidden to print.\n",id,child[id]);
                                     flag = kill(child[id], SIGUSR2);
                                 }
                             }else{
                                 printf("pid(C<%d>) = 0!\n", id);
                             }
                         }
                         if(flag==-1){
                             perror("kill");
                             exit(-1);
                         }
                         break;
                     }
            case 'l':{  
                         for (int i = 0; i < MAX_CHILD_AMT; i++) {
                             if(child[i] != 0){ 
                                 printf("\t[%d]-'%d'\n",i,child[i]);
                             }else{
                                 printf("\t[%d]-none\n",i);
                             }
                         }
                         break;
                     }

            case '+':{
                         if(counter >= MAX_CHILD_AMT){
                            printf("\nERROR: can't make more children! max amt = %d\n\n", MAX_CHILD_AMT);
                         }

                         //forking
                         pid = fork();

                         switch (pid) {
                             case -1:{                           //fork failed
                                         (void)perror("fork");
                                         (void)free(name);
                                         (void)exit(EXIT_FAILURE);

                                     }

                             case 0:{                            //for forked

                                        //update process name
                                       // for(i = 0; i < MAX_CHILD_AMT && child[i] != 0 && child[i] != getpid(); i++);
                                        name[5] = '0' + counter;
                                        argv[0] = name;
                                        flag = execve(CHILD_PATH, argv, envp);
                                        if(flag == -1){
                                            (void)printf("execve error:%s\n", strerror(errno));
                                            (void)exit(EXIT_FAILURE);
                                        }
                                        exit(1);
                                        break;
                                    }

                             default:{       //for parent
                                         printf("created C%d-%d\n",counter,pid);
                                         child[counter] = pid;
                                         break;
                                     }      
                         }
                         counter = -1;
                         break;
                     }
            case '-':{
                         pid_t max_pid = -1;
                         int max_pid_id = -1;
                         flag = 0;
                         for(int i = 0; i < MAX_CHILD_AMT; i++){
                             //SELECT child WHERE MAX(pid)
                             if(child[i] > max_pid && child[i] != 0){                              
                                 max_pid = child[i];
                                 max_pid_id = i;
                             }
                         }
                         if(max_pid > 0 
                                 && max_pid_id < MAX_CHILD_AMT 
                                 && max_pid_id >= 0
                           ){
                             flag = kill(max_pid,SIGINT);
                             printf("killed C%d-%d\n",max_pid_id,max_pid);
                             child[max_pid_id] = 0;
                         }else{
                             printf("no more children to delete\n");
                         }
                         if(flag==-1){
                             perror("kill");
                             printf("id = '%d', max_pid='%d'\n",max_pid_id,max_pid);
                             exit(-1);
                         }
                         counter = -1;
                         break;
                     }
            default:{
                        printf("unknown command\n");
                        break;

                    }
        } 

    }while(1);
    return 0;
}
