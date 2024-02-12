#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <locale.h>
#define OPTMASK "ldfs"
#define DEFAULT_DIR "./"
int main(int argc, char** argv){

	//error checker
	int flag = 0;
	
	//sys vars
	DIR* dirp = NULL;
	struct dirent* ent = NULL;
	struct stat entstat;
	
	//for getopt
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
					perror("unknown opt param");
					exit(1);
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
	if(sflag==1){
		setlocale(LC_COLLATE,"en-US");
	}	
	if((fflag==0) && (dflag==0) && (lflag==0)){
		fflag = 1;
		dflag = 1;
		lflag = 1;
	}
	ent=readdir(dirp);
	
	while(ent!=NULL){
		char* path = realpath(ent->d_name, NULL);
		if(path!=NULL){
			flag=lstat(ent->d_name,&entstat);
			if(flag==-1){
				perror("stat failed");
				exit(1);
			}
			if	((S_ISREG(entstat.st_mode) && fflag) || 
				 (S_ISLNK(entstat.st_mode) && lflag) || 
				 (S_ISDIR(entstat.st_mode) && dflag)
			){			
				printf("%s\n", path);
			}
		}
			ent = readdir(dirp);
	}
		

	flag = closedir(dirp);
	if(flag==-1){
		perror("DIR failed to close!");
		exit(1);
	}

	return 0;
}