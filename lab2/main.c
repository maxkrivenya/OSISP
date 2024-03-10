#include "lab2.h"
#include <errno.h>
#include <string.h>
#define DEFAULT_PATH "child_file_path/child"
#define CHILD_PATH_VAR_NAME 10
#define CHILD_NAME "child"
#define NAME_SIZE 10

int main(int argc, char *argv[], char *envp[]){
    //  setenv(CHILD_PATH, DEFAULT_PATH, 1);
    pid_t pid;
    int flag = 0;
    char get;
    int counter = 0;
    char** argv2 = (char**)calloc(4,sizeof(char*));
    if(argv2==NULL){
        (void)printf("calloc error. exiting\n");
        (void)exit(0);
    }
    argv2[3] = NULL;

    argv2[1] = (char*)calloc(2,sizeof(char)); 
    if(argv2[1]==NULL){ 
        (void)printf("calloc error. exiting\n"); 
        (void)exit(0);
    }

    argv2[2] = (char*)calloc(POSIX_PATH_MAX,sizeof(char)); 
    if(argv2[2]==NULL){ 
        (void)printf("calloc error. exiting\n"); 
        (void)exit(0);
    }
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        (void)perror("signal");
        (void)exit(EXIT_FAILURE);
    }

    char* name = (char*)calloc(NAME_SIZE,1); //init "CHILD%d%d" string
    (void)strcat(name,  CHILD_NAME);

    do{
        (void)fflush(stdin);
        get = getc(stdin);  //read command symbol
        (void)getc(stdin);        //remove \n from stdin

        if(get == 'q'){             //exit on command     
            (void)puts("parent exit");
            (void)free(name);
            (void)exit(EXIT_SUCCESS);
        }
        argv2[1][0] = get;
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
                                        child_path = get_path_from_getenv("CHILD_PATH");
                                        (void)printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '&':{
                                        child_path = get_path_from_env(envp, "CHILD_PATH");
                                        (void)printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '*':{
                                        child_path = get_path_from_environ("CHILD_PATH");
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
                           argv2[0] = name;   
                           flag = execve(child_path, argv2, envp);
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
                            (void)free(argv2[3]);
                            (void)free(argv2[2]);
                            (void)free(argv2[1]);
                            (void)free(argv2);
                            (void)free(name);
                            (void)exit(EXIT_FAILURE);
                        }
                    }
        }
    }while(get != 'q');

    return 0;
}
