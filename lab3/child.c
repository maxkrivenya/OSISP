#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[]){
    struct combination{
        int zero;
        int one;
        int ten;
        int eleven;
    };
    struct combination combination_t = {0,0,0,0};

    struct timespec first_t = {0,5000000};
    struct timespec second_t = {0,50};
    struct timespec result_t = {0,0};
    pid_t pid;


    for(int i = 0; i < 101; i++){

        int   pipefd[2];            // pipe for process communication,
        if (pipe(pipefd) == -1) {   // [0] = read, [1] = write
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();       // child - sleep, parent - struct randomizer
        
        switch (pid) {
            case -1:{                           //fork failed
                        (void)perror("fork");
                        (void)exit(EXIT_FAILURE);

                    }

            case 0:{   //for child
                       close(pipefd[0]);    // close read end of pipe
                       char message = 'a';
                       nanosleep(&first_t,&second_t);   //sleep
                       write(pipefd[1], &message, sizeof(char)); //wake up, write message to parent
                       close(pipefd[1]);    // close write end of pipe
                       exit(1);
                       break;
                   }

            default:{       //for parent
                        close(pipefd[1]); //close write end of pipe
                        char buffer = 'b';
                        while(buffer != 'a'){   //while child doesnt wake up
                            read(pipefd[0],&buffer,1);
                            result_t.tv_sec  = rand()&1;    //random struct
                            result_t.tv_nsec = rand()&1;
                        }

                        if(result_t.tv_sec==0){         //when child woke up, statistics
                            if(result_t.tv_nsec==0){
                                combination_t.zero++;
                            }
                            else{
                                combination_t.one++;
                            }
                        }else{
                            if(result_t.tv_nsec==0){
                                combination_t.ten++;
                            }else{
                                combination_t.eleven++;
                            }
                        }
                        close(pipefd[0]);       //close read end of pipe
                        break;
                    }
        }   // switch
    }   // i = 0 to 101
    printf("\nname:%s\npid:%d\nppid:%d\n\t00:%d\n\t01:%d\n\t10:%d\n\t11:%d\nend\n",
            argv[0],
            getpid(),
            getppid(),
            combination_t.zero,
            combination_t.one,
            combination_t.ten,
            combination_t.eleven);
    exit(1);
    return 0;
}

