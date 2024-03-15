
#include <errno.h>
#include <string.h>
#define CHILD_PATH "./child"
#define CHILD_NAME "child"
#define NAME_SIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#define POSIX_PATH_MAX 512
extern char** environ;

int main(int argc, char *argv[], char *envp[]){

    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        (void)perror("signal");
        (void)exit(EXIT_FAILURE);
    }

    char str[50] = "ps -o pid --ppid ";
    char kill_children[20] = "pkill -P ";
    char ppid [7];
    sprintf(ppid,"%d",getpid());
    strcat(str,ppid);
    strcat(kill_children, ppid);
    pid_t pid;
    int flag = 0;
    char get;
    char bufclear;
    int counter = 0;

    char* name = (char*)calloc(NAME_SIZE,1); //init "CHILD%d%d" string

    (void)strcat(name,  CHILD_NAME);

    (void)fflush(stdin);
    do{ 
        get = getc(stdin);  //read command symbol
        bufclear = getc(stdin);        //remove \n from stdin
        if(bufclear != '\n'){
            get = bufclear;
        }

        switch(get){
            case 'q':{
                         free(name);
                         (void)puts("parent exit");
                         kill(0,SIGINT);
                         (void)exit(1);
                         break;
                     }
            case 'l':{
                         system(str);
                         break;
                     }
            case 'k':{
                         (void)puts("killing everyone...\n");
                         system(kill_children);
                         (void)puts("everyone is dead.\n");
                         break;
                     }
            default:{
                        //forking
                        pid = fork();

                        switch (pid) {
                            case -1:{                           //fork failed
                                        (void)perror("fork");
                                        (void)free(name);
                                        (void)exit(EXIT_FAILURE);

                                    }

                            case 0:{                            //for forked
                                       if(-1==setsid()){
                                           return -1;
                                       }
                                       //update process name
                                       name[5] = '0' + counter / 10;
                                       name[6] = '0' + counter % 10;
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
                                        (void)printf("child : %jd\n", (intmax_t) pid);
                                      /*  (void)wait(&flag);
                                        if (flag == -1) {
                                            (void)perror("waitpid");
                                            (void)free(name);
                                            (void)exit(EXIT_FAILURE);
                                        }
                                        */
                                        break;
                                    }      
                        }
                    }
        } 

    }while(1);
    return 0;
}
