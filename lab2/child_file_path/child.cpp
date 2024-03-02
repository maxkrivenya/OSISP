#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[]){
    (void)printf("\tname=%s\n", argv[0]);
    (void)printf("\tpid=%d\n", getpid());
    (void)printf("\tppid=%d\n", getppid());
    (void)printf("child exit\n");
    (void)exit(EXIT_SUCCESS);
}
