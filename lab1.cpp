#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

void printEntry(struct dirent* ent, char type){
	if(ent==NULL){return;}
	int fildes = open(ent.name, 0_RDWR);
	struct stat entstat;
	int status = fstat(fildes, &entstat);
	if(status==-1){
		perror("fstat failed");
		exit(1);
	}
	switch(type){
		case 'f':{
			if(S_ISREG(entstat.st_mode)){
				printf("\nType:'%c'\tName:'%s'\n",'a'+(ent->d_type-1),ent->d_name);
			}
			break;
		}
		case 'd':{
			if(S_ISDIR(entstat.st_mode)){
				printf("\nType:'%c'\tName:'%s'\n",'a'+(ent->d_type-1),ent->d_name);
			}
			break;
		}
		case 'l':{
			if(S_ISLNK(entstat.st_mode)){
				printf("\nType:'%c'\tName:'%s'\n",'a'+(ent->d_type-1),ent->d_name);
			}
			break;
		}
		default:{
			printf("\nweird printEntry type: '%c'", type);
			break;
		}
	}
	status = close(fildes);
	if(status == -1){
		perror("file not closed!!!");
		exit(1);
	}
	return;
}

int main(int argc, char** argv){
	DIR* dirp = NULL;
	int flag = 0;
	size_t i = 0;
	size_t j = 0;
	size_t curr_arg_len=0;
	size_t mask_size = 0;
	struct dirent* ent = NULL;
	struct stat entstat;
	bool fflag=true;
	bool dflag=true;
	bool lflag=true;

	for(i=1; i < argc; i++){
		if(argv[i][0] != ' '){
			dirp = opendir(argv[i]);
		}
		if(dirp!=NULL){	
			perror("DIR open error");
			exit(1);
		}	

	for(i=1;i<argc;i++){	
		curr_arg_len=strlen(argv[i]);
		if(curr_arg_len<=1){
			perror("short arg");
			exit(1);
		}
		if(argv[i][0]=='-'){
			for(j=1;j<curr_arg_len;j++){
				switch(argv[i][j]) {
					case 'f':{
						if(fflag==true){
							fflag=false;
							do{
								ent = readdir(dirp);
								printEntry(ent, 'f');
							}while(ent != NULL);
							rewinddir(dirp);
						}		
					}
				}
			}
		}

	}

	
	flag = dirclose(dirp);
	if(flag==-1){
		perror("DIR failed to close!");
		exit(1);
	}
	return 0;
}
