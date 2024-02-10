#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#define PATHLENGTH 40
#define SCRIPTNAME 24
#define SCRIPT 80
#define MASKLENGTH 5

int main(int argc, char** argv){    
    char mask[MASKLENGTH] = "";
    char path[PATHLENGTH]= "";  
    char scriptname[SCRIPTNAME] = "./lab1_script.sh";
    size_t size_of_arg = 0;
    size_t amt_of_masks = 0;
    size_t i = 0;
    size_t j = 0;
    mask[MASKLENGTH-1]='\0';
    for(i = 1; i < argc; i++){
	size_of_arg = strlen(argv[i]);
	if(argv[i][0]=='-'){
	    for(j = 1; j < size_of_arg; j++){
		switch(argv[i][j]){
		    case 'f':{
			mask[amt_of_masks]='-';
			amt_of_masks++;
			break;
		    }
		    case 'd':{
			mask[amt_of_masks]='d';
			amt_of_masks++;
			break;
		    }
		    case 'l':{
			mask[amt_of_masks]='l';
			amt_of_masks++;
			break;
		    }
		    default:{
			printf("Error argv value:'%s', %d , %d", argv[i], j, size_of_arg);
			exit(1);
		    }
		}
	    }
	}else{
	    for(j = 0; j < PATHLENGTH && j < size_of_arg; j++){
	        path[j] = argv[i][j];
	    }
	    path[j] = '\0';
        }
    }
    char script[SCRIPT] = "";
    for(i = 0; i < SCRIPTNAME && scriptname[i] != '\0'; i++){
	script[i] = scriptname[i];
    } 
    script[i]=' ';  
    i++;
//    printf("mask == '%s'\0", mask);
    if(mask[0] == '\0'){
	mask[0]='-';
	mask[1]='l';
	mask[2]='d';
	mask[MASKLENGTH-1] = '\0';
    }
//    printf("mask == '%s'\0",mask);
    for(j = 0; j < MASKLENGTH && mask[j] != '\0'; i++ && j++){
	script[i] = mask[j];
    }    
    script[i]=' ';
    i++;
//    printf("path == '%s'\0",path);
    if(path[0]=='\0'){
	path[0]='~';
	path[1]='\0';
    }
//    printf("path == '%s'\0",path);
    for(j = 0; j < PATHLENGTH  && path[j] != '\0'; i++ && j++){
//	printf("where is my fucking ~\0");
	script[i] = path[j];
//	printf("%s", script);
    }    
    i++;
    script[i]='\0';
    printf("%s", script);
    
    system(script);
    return 0;
}