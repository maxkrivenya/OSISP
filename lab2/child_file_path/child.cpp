#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "./../lab2.h"
#define DEFAULT_FILE_PATH "/home/myra/linux4sem/osisp/lab2/envlist.txt\0"
//#define DEFAULT_FILE_PATH "osisp/lab2/envlist.txt\0"
//#define DEFAULT_FILE_PATH "defenvlist.txt"
#define MAX_ENV_VAR_NAME 30 

int main(int argc, char** argv, char** envp){
    if(argc < 2){
        (void)printf("not enough parameters: argc='%d'. exiting\n", argc);
        (void)exit(0);
    }
    printf("argv:\n\t%s\n\t%s\n\t%s\nargv end\n\n",argv[0],argv[1],argv[2]);
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
        fptr = fopen(argv[2],"r");
    }
    if(fptr==NULL){
        fptr = fopen(DEFAULT_FILE_PATH,"r");
    }
    if(fptr==NULL){
        (void)printf("no files found. exiting\n");
        (void)printf("errno:%s\n", strerror(errno));
        (void)exit(0);

    }
        
    while(!feof(fptr)){
        (void)fgets(str, MAX_ENV_VAR_NAME, fptr);
        if(str==NULL){
            (void)printf("fgets error. exiting\n");
            (void)exit(0);
        }
        str[strlen(str)-1] = '\0';
        (void)printf("\t\t%s=%s\n", str, getenv(str));
    }
    flag = fclose(fptr);
    if(flag==EOF){
        printf("file close error.\n");
        exit(0);
    }
    (void)printf("child exit\n");
    (void)exit(EXIT_SUCCESS);
}
