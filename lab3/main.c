
#include <errno.h>
#include <string.h>
#define DEFAULT_PATH "child_file_path/child"
#define CHILD_PATH_VAR_NAME 10
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
    
    //getting environ size
    size_t count = 0;
    
    pid_t pid;
    int flag = 0;
    char get;
    int counter = 0;

    char* name = (char*)calloc(NAME_SIZE,1); //init "CHILD%d%d" string

    (void)strcat(name,  CHILD_NAME);

    do{
        (void)fflush(stdin);
        get = getc(stdin);  //read command symbol
        (void)getc(stdin);        //remove \n from stdin

        if(get == 'q'){             //exit on command     
            (void)puts("parent exit");
            (void)exit(EXIT_SUCCESS);
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
                       char* child_path = NULL;
                       switch (get) {
                           case '+':{
                                        (void)printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '&':{
                                        (void)printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '*':{
                                        (void)printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           default:{
                                       (void)printf("unrecognised symbol\n");
                                       break;
                                   }
                       }


                       if(get == '+' || get == '&' || get == '*'){
                           //update process name 
                           name[5] = '0' + counter / 10;
                           name[6] = '0' + counter % 10;
                           flag = execve(child_path, argv, envp);
                           if(flag == -1){
                               (void)printf("execve error:%s\n", strerror(errno));
                               (void)exit(EXIT_FAILURE);
                           }
                       }
                       else{
                           (void)printf("exiting\n");
                           (void)exit(1);
                       }
                       break;
                   }

            default:{       //for parent
                        counter++;
                        (void)printf("child : %jd\n", (intmax_t) pid);
                        (void)wait(&flag);
                        if (flag == -1) {
                            (void)perror("waitpid");
                            (void)free(name);
                            (void)exit(EXIT_FAILURE);
                        }
                    }
        }
    }while(get != 'q');

    return 0;
}
