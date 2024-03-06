#include "lab2.h"
#define CHILD_PATH "CHILD_PATH"
#define CHILD_PATH_VAR_NAME 10
#define CHILD_NAME "child"
#define NAME_SIZE 10

int main(int argc, char **argv, char **envp){
    pid_t pid;
    int flag = 0;
    char get;
    int counter = 0;

    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    char* name = (char*)calloc(NAME_SIZE,1); //init "CHILD%d%d" string
    (void)strcat(name,  CHILD_NAME);

    do{
        fflush(stdin);
        get = getc(stdin);  //read command symbol
        getc(stdin);        //remove \n from stdin
        
        if(get == 'q'){             //exit on command     
            puts("parent exit");
            free(name);
            exit(EXIT_SUCCESS);
        }

        pid = fork();

        switch (pid) {
            case -1:{                           //fork failed
                        perror("fork");
                        free(name);
                        exit(EXIT_FAILURE);

                    }

            case 0:{                            //for forked
                       char* child_path = NULL;
                       switch (get) {
                           case '+':{
                                        child_path = get_path_from_getenv(CHILD_PATH);
                                        printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '&':{
                                        child_path = get_path_from_env(envp, CHILD_PATH);
                                        printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '*':{
                                        child_path = get_path_from_environ(CHILD_PATH);
                                        printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           default:{
                                       printf("unrecognised symbol\n");
                                       break;
                                   }
                       }
                       
                        
                       if(get == '+' || get == '&' || get == '*'){
                           //update process name 
                           name[5] = '0' + counter / 10;
                           name[6] = '0' + counter % 10;
                           argv[0] = name;            
                        
                           flag = execve(child_path, argv, envp);
                           if(flag == -1){
                               printf("execve error\n");
                               exit(EXIT_FAILURE);
                           }
                       }
                       else{
                           printf("exiting\n");
                           exit(1);
                       }

                   }

            default:{       //for parent
                        counter++;
                        printf("child : %jd\n", (intmax_t) pid);
                        wait(&flag);
                        if (flag == -1) {
                            perror("waitpid");
                            free(name);
                            exit(EXIT_FAILURE);
                        }
                    }
        }
    }while(get != 'q');

    return 0;
}
