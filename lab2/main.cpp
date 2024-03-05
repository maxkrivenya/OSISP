#include <bits/fs_fwd.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <cstring>
#define CHILD_PATH "CHILD_PATH"
#define CHILD_PATH_VAR_NAME 10
#define CHILD_NAME "child"
#define NAME_SIZE 10


//EXTRACT value FROM name=value
char* get_path_from_var(char* a){
    if(strlen(a) < CHILD_PATH_VAR_NAME){
    printf("too short\n");
    return NULL;
    }
int i = CHILD_PATH_VAR_NAME+1;
    for(; a[i] != '\0'; i++){
        a[i-CHILD_PATH_VAR_NAME-1] = a[i];
    }
    a[i-CHILD_PATH_VAR_NAME-1] = '\0';
    return a;
}

//SEE IF name==CONST in name=value
int begins(const char *a){
   int i = 0;
    for(; i < CHILD_PATH_VAR_NAME && i < strlen(a); i++){
        if(a[i] != CHILD_PATH[i]){
            return 0;
        }
    }
    if(a[i]=='=' && i < strlen(a)){
        return 1;
    }
    return 0;
}

//parse environ until i get something that begins with CONST
char* get_path_from_environ(){
    for(int i = 0; environ[i] != NULL; i++){
        if(begins(environ[i])){
            return get_path_from_var(environ[i]);
        }
    }
    return NULL;
}

//parse envp[] until i get something that begins with CONST
char* get_path_from_env(char* envp[]){
    for (char **env = envp; *env != 0; env++){
        if(begins(*env)){
             return get_path_from_var(*env);
        }
    }
    return NULL;
}

//getenv() with a check 
char* get_path_from_getenv(){
    char* child_path = getenv(CHILD_PATH);

    if(child_path==NULL){
        printf("ur env var is fucked up\n");
        return NULL;
    }
    return child_path; 
}


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
                                        child_path = get_path_from_getenv();
                                        printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '&':{
                                        child_path = get_path_from_env(envp);
                                        printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '*':{
                                        child_path = get_path_from_environ();
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
