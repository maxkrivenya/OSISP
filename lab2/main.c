#include "lab2.h"
#define DEFAULT_PATH "child_file_path/child"
#define CHILD_PATH_VAR_NAME 10
#define CHILD_NAME "child"
#define NAME_SIZE 10

int main(int argc, char *argv[], char *envp[]){
    //getting environ size
    size_t count = 0;
    while ( environ[count] != 0 )
    {
        ++count;
    }
    //copying environ to my_environ
    char ** my_environ = malloc( sizeof( char * ) * count );
    if(memcpy( my_environ, environ, sizeof( char * ) * count ) == NULL){
        perror("memcpy");
        exit(-1);
    }
    //sorting my_environ
    (void)qsort( my_environ, count, sizeof( char * ), comp );
    //printing out sorted environ
    for ( size_t i = 0; i < count; ++i )
    {
        puts( my_environ[i] );
    }
    (void)printf("end of environ\n\n");
    
    pid_t pid;
    int flag = 0;
    char get;
    int counter = 0;

    //argv for child
    char* argv2[3];
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
            (void)free(argv2[2]);
            (void)free(argv2[1]);
            (void)free(name);
            (void)exit(EXIT_SUCCESS);
        }
    
        //putting command into child argv
        argv2[1][0] = get;

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
                                        child_path = get_path_from_getenv("CHILD_PATH");
                                        //(void)printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '&':{
                                        child_path = get_path_from_env(envp, "CHILD_PATH");
                                        //(void)printf("CHILD_PATH=%s\n", child_path);
                                        break;
                                    }
                           case '*':{
                                        child_path = get_path_from_environ("CHILD_PATH");
                                        //(void)printf("CHILD_PATH=%s\n", child_path);
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
                            (void)free(argv2[2]);
                            (void)free(argv2[1]);
                            (void)free(name);
                            (void)exit(EXIT_FAILURE);
                        }
                    }
        }
    }while(get != 'q');

    return 0;
}
