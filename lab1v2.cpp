#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define OPTMASK "fdls"
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
	
	if(!fflag && !dflag && !lflag){
		fflag = 1;
		dflag = 1;
		lflag = 1;
	}
	ent=readdir(dirp);
	
	while(ent!=NULL){
		flag=lstat(ent->d_name,&entstat);
		if(flag==-1){
			perror("stat failed");
			exit(1);
		}
		if(S_ISREG(entstat.st_mode) && fflag){			//if file, print
			printf("\nFIL\t'%s'\n",ent->d_name); 
		}

		if(S_ISLNK(entstat.st_mode) && lflag){			//if lnk, print
			printf("\nLNK\t'%s'\n",ent->d_name);
		}
		if(S_ISDIR(entstat.st_mode) && dflag){			//if dir, print
			printf("\nDIR\t'%s'\n",ent->d_name);
//			pid_t childpid = fork();
//			if(childPid==0){
//				execve("./lab1v2",
//				printf("\nChild process %d ended.\n",pid(childPid);
//				exit(0);
//			}
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
