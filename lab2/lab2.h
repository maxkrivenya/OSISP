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

//EXTRACT value FROM name=value
char* get_path_from_var(char* env, int length_of_name){
    if(env == NULL){
    printf("NULL received\n");
    return NULL;
    }
    int i = length_of_name+1;
    for(; env[i] != '\0'; i++){
        env[i-length_of_name-1] = env[i];
    }
    env[i-length_of_name-1] = '\0';
    return env;
}

//SEE IF name==CONST in name=value
int begins(char *env, char* name){
   if(env == NULL){
    printf("env==null\n");
    return -1;
   }
      if(name == NULL){
    printf("name==null\n");
    return -1;
   }
   int i = 0;
    for(; i < strlen(name) && i < strlen(name); i++){
        if(env[i] != name[i]){
            return 0;
        }
    }
    if(env[i]=='=' && i < strlen(env)){ //maybe remove strlen env?
        return 1;
    }
    return 0;
}

//parse environ until i get something that begins with name
char* get_path_from_environ(char* name){
    for(int i = 0; environ[i] != NULL; i++){
        if(begins(environ[i], name)){
            return get_path_from_var(environ[i], strlen(name));
        }
    }
    return NULL;
}

//parse envp[] until i get something that begins with name
char* get_path_from_env(char* envp[], char* name){
    for (char **env = envp; *env != 0; env++){
        if(begins(*env, name)){
             return get_path_from_var(*env, strlen(name));
        }
    }
    return NULL;
}

//getenv() with a check 
char* get_path_from_getenv(char* name){
    char* var_value = getenv(name);
    if(var_value==NULL){
        printf("ur env var is fucked up\n");
        return NULL;
    }
    return var_value; 
}
