#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#define OPTMASK "fdls"
#define DEFAULT_DIR "./"
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
	
	char opt;
	//flags for optparameters
	bool fflag=false;
	bool dflag=false;
	bool lflag=false;
	bool sflag=false;	
	do{
		opt = getopt(argc,argv, OPTMASK);
		if(opt!=-1){
		switch(opt){
			case 'f':{
				fflag=true;
				break;
			}
			case 'd':{
				dflag=true;
				break;
			}
			case 'l':{
				lflag=true;
				break;
			}
			case 's':{
				sflag=true;
				break;
			}
			default:{
				//perror("unknown opt param");
				//exit(1);
				printf("\nunknown opt param:'%c'\n",opt);
				break;
			}
		}
		}
	}while(opt!=-1);

	if(optind<argc){
		dirp=opendir(argv[optind]);
	}
	if(dirp==NULL){
		dirp=opendir(DEFAULT_DIR);
	}
	if(dirp==NULL){
		perror("DIR open error");
		exit(1);
	}
	

	flag = closedir(dirp);
	if(flag==-1){
		perror("DIR failed to close!");
		exit(1);
	}

	return 0;
}
