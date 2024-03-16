#include "header.h"
#include <stdio.h>

int main(int argc, char* argv[]){
    if(argc < 1){return -1;}
    
    signal(SIGUSR1,sig1_handler);
    signal(SIGUSR2,sig2_handler);

    struct timespec first_t = {0,10000000};
    struct timespec second_t = {2,50};
    struct timespec result_t = {0,0};

    pid_t pid   = -1;
    
    int flag    = 0;
    int i       = 0;
    
    for(;;){
        struct combination combination_t = {0,0,0,0};
        
        for(i = 0;i < 200;i++){
            int   pipefd[2];   

            // pipe for process communication,
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
                           flag = nanosleep(&first_t,&second_t);   //sleep
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
                            close(pipefd[0]);       //close read end of pipe'
                        }
                        break;
            }   // switch
        }   // i = 0 to 101
        for(;print_allowed == -1;);

        if(print_allowed==1){
            printf("%s %d %d 00:%d 01:%d 10:%d 11:%d\n",
                    argv[0],
                    getpid(),
                    getppid(),
                    combination_t.zero,
                    combination_t.one,
                    combination_t.ten,
                    combination_t.eleven)
                ;
        }
    }
    printf("%s exiting.\n",argv[0]);
    exit(1);
}
