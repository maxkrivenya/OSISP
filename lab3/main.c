#include "header.h"
#include <signal.h>
#include <stdio.h>
#define COMMAND_LENGTH 6 
#define MAX_CHILD_AMT 10

int main(int argc, char *argv[], char *envp[]){
    pid_t child[MAX_CHILD_AMT];
    for(int i = 0; i < MAX_CHILD_AMT; i++){
        child[i] = 0;
    }
    int i = 0;
    printf("volatile=%d\n",print_allowed);
    if(argc < 1){return -1;}
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        (void)perror("signal");
        (void)exit(EXIT_FAILURE);
    }

    char list[50] = "ps -o pid --ppid ";
    char kill_children[20] = "pkill -P ";
    char ppid [7];
    sprintf(ppid,"%d",getpid());
    strcat(list,ppid);
    strcat(kill_children, ppid);
    pid_t pid;
    int flag = 0;
    int counter = 0;
    pid_t max_pid = 0;

    char* name  = (char*)calloc(NAME_SIZE,1); //init "CHILD%d%d" string
    char* get  = (char*)calloc(COMMAND_LENGTH,1); //init g<K> string
    char comm;

    int id = 0;
    
    (void)strcat(name,  CHILD_NAME);

    (void)fflush(stdin);
    do{ 
        get = fgets(get, COMMAND_LENGTH-1, stdin); //comm="X<Y>"
        comm = get[0];
        if(get[1] == '<' && isdigit(get[2])){
            id = get[2] - '0';
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
                         (void)puts("killing everyone...\n");
                         for(int i = 0; i < MAX_CHILD_AMT; i++){
                             if(child[i] != 0){
                                 kill(child[i],SIGINT);
                                 child[i] = 0;
                             }
                         }
                         (void)puts("everyone is dead.\n");
                         break;
                     }
            case 'g':{
                         signal(SIGUSR1,sig1_handler);
                         if(id < 0){
                             for(i = 0; i < MAX_CHILD_AMT; i++){
                                 if(child[i] != 0){
                                     printf("allowing %d\n",child[i]);
                                     flag = kill(child[i],SIGUSR1);
                                 }
                             }
                         }else{
                             if(id < MAX_CHILD_AMT){
                                 if(child[id] != 0){
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
                             for(i = 0; i < MAX_CHILD_AMT; i++){
                                 flag = kill(child[i],SIGUSR2);
                             }
                         }else{
                             if(id < MAX_CHILD_AMT){
                                 if(child[id] != 0){
                                     flag = kill(child[id], SIGUSR2);
                                 }
                             }else{
                                 printf("pid(C<%d>) = 0!\n", id);
                             }
                         }
                         printf("volatile=%d, forbidden\n",print_allowed);
                         if(flag==-1){
                             perror("kill");
                             exit(-1);
                         }
                         break;
                     }
            case 'l':{  
                         printf("child list:\n");
                         for (int i = 0; i < MAX_CHILD_AMT; i++) {
                             if(child[i]!=0){
                                 printf("\t[%d]-'%d'\n",i,child[i]);
                             }
                         }
                         printf("children listed.\n");
                         break;

                     }

            case '+':{
                         if(counter >= MAX_CHILD_AMT){
                             //(void)printf("cant create more children\n");
                             //break;
                             while(counter >= MAX_CHILD_AMT){
                                 counter -= MAX_CHILD_AMT;
                             }
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
                                         counter++;
                                         for(i = 0; i < MAX_CHILD_AMT && child[i] != 0; i++);
                                         child[i] = pid;
                                         //(void)printf("child : %jd\n", (intmax_t) pid);

                                         break;
                                     }      
                         }
                         break;
                     }
            case '-':{
                         pid_t max_pid = -1;
                         int max_pid_id = -1;
                         flag = 0;
                         for(int i = 0; i < MAX_CHILD_AMT; i++){
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
                             child[max_pid_id] = 0;
                         }else{
                             printf("no more children to delete\n");
                         }
                         if(flag==-1){
                             perror("kill");
                             printf("id = '%d', max_pid='%d'\n",max_pid_id,max_pid);
                             exit(-1);
                         }
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
