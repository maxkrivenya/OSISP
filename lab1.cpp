#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

void printEntry(struct dirent* ent, char type){			//function for printing a single dir entry
	if(ent==NULL){
		return;
	}
	struct stat entstat;
	int status = lstat(ent->d_name, &entstat);	//get entry status to see its filetype
	if(status==-1){
		perror("fstat failed");
		exit(1);
	}
	switch(type){		//depending on what parameter is passed
		case 'f':{
			if(S_ISREG(entstat.st_mode)){			//if file, print
				printf("\nFIL\t'%s'\n",ent->d_name); 
			}
			break;
		}
		case 'd':{
			if(S_ISDIR(entstat.st_mode)){			//if dir, print
				printf("\nDIR\t'%s'\n",ent->d_name);
			}
			break;
		}
		case 'l':{
			if(S_ISLNK(entstat.st_mode)){			//if lnk, print
				printf("\nLNK\t'%s'\n",ent->d_name);
			}
			break;
		}
		default:{
			printf("\nweird printEntry type: '%c'", type);
			break;
		}
	}
	return;
}

int main(int argc, char** argv){
	
	//error checker
	int flag = 0;
	
	//iterators
	size_t i = 0; 
	size_t j = 0;

	//argv[i] size holder
	size_t curr_arg_len=0;
	
	//sys vars
	DIR* dirp = NULL;
	struct dirent* ent = NULL;
	struct stat entstat;
	
	//flags for optparameters
	bool fflag=true;
	bool dflag=true;
	bool lflag=true;
	
	//find dir path in parameters [1/-1]
	for(i=1; i < argc; i++){
		if(argv[i][0] != '-' && dirp==NULL){
			dirp = opendir(argv[i]);
		}

	}
	if(dirp==NULL){
		dirp=opendir("./");	  //if invalid/none passed, open current
	}
	if(dirp==NULL){
		perror("dir open error"); //error check
		exit(1);
	}

	for(i=1;i<argc;i++){				//looking for opt pars in argv
		curr_arg_len=strlen(argv[i]);
		if(curr_arg_len<=1){
			perror("short arg");
			exit(1);
		}
		if(argv[i][0]=='-'){			//if opt par
			for(j=1;j<curr_arg_len;j++){	//see what it includes
				switch(argv[i][j]) {	//for each data type: switch flag to false, for each file in dir: call print with current type
					case 'f':{
						if(fflag==true){
							fflag=false;
							do{
								ent = readdir(dirp);
								printEntry(ent, 'f');
							}while(ent != NULL);
							rewinddir(dirp);
						}
						break;
					}
					case 'd':{
						if(dflag==true){
							dflag=false;
							do{
								ent = readdir(dirp);
								printEntry(ent, 'd');
							}while(ent != NULL);
							rewinddir(dirp);
						}
						break;
					}
					case 'l':{
						if(lflag==true){
							lflag=false;
							do{
								ent = readdir(dirp);
								printEntry(ent, 'l');
							}while(ent != NULL);
							rewinddir(dirp);
						}
						break;
					}
								 
					default:{
						printf("\nNaF - '%c'\n", argv[i][j]);
						break;
					}
				}
			}
		}

	}

	
	flag = closedir(dirp);			//close folder
	if(flag==-1){
		perror("DIR failed to close!");
		exit(1);
	}

	return 0;
}
