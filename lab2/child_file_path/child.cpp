#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "./../lab2.h"
#define DEFAULT_FILE_PATH "~/linux4sem/osisp/lab2/envlist.txt"
#define MAX_ENV_VAR_NAME 30 

int main(int argc, char* argv[], char* envp[]){
    if(argc < 2){
        (void)printf("not enough parameters: argc='%d'. exiting\n", argc);
        (void)exit(0);
    }

    int flag = 0;
    char* str = (char*)calloc(MAX_ENV_VAR_NAME, 1);
    if(str==NULL){
        (void)printf("calloc error. exitibg\n");
        (void)exit(0);
    }
    (void)printf("\tname=%s\n", argv[0]);
    (void)printf("\tpid=%d\n", getpid());
    (void)printf("\tppid=%d\n", getppid());
    
    FILE* fptr = NULL;
    if(argv[2] != NULL){
        fptr = fopen(argv[1],"r");
    }
    if(fptr==NULL){
        fptr = fopen(DEFAULT_FILE_PATH,"r");
    }
    if(fptr==NULL){
        (void)printf("no files found. exiting\n");
        (void)exit(0);
    }
        
    while(!feof(fptr)){
        (void)fgets(str, MAX_ENV_VAR_NAME, fptr);
        if(str==NULL){
            (void)printf("fgets error. exiting\n");
            (void)exit(0);
        }
        (void)printf("%s=%s",str,get_path_from_getenv(str));
    }

    (void)printf("child exit\n");
    (void)exit(EXIT_SUCCESS);
}
