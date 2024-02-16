#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <locale.h>
#include <cstring>
#define OPTMASK "ldfs"
#define DEFAULT_DIR "./"
#define POSIX_PATH_MAX 512


void actually_print_dir(const char* path, bool fflag, bool dflag, bool lflag, bool sflag){
	//error checker
	int flag = 0;
	struct stat entstat;	
	struct dirent **namelist;
	int n;
	if(sflag){
		n = scandir(path, &namelist, 0, alphasort);
	}else{
		n = scandir(path, &namelist, 0, 0);
	}

	if (n < 0){
		perror("scandir");
		return;
	}
//	currdir = getcwd(currdir, 80);
	while (--n) {
		char entry[POSIX_PATH_MAX] = {0};
		(void)strcat(entry,path);	
		(void)strcat(entry,"/");
		(void)strcat(entry,namelist[n]->d_name);
		flag=lstat(entry,&entstat);
		if(flag==-1){
			perror("stat failed");
			exit(1);
		}
		if	((S_ISREG(entstat.st_mode) && fflag) || 
			 (S_ISLNK(entstat.st_mode) && lflag) || 
			 (S_ISDIR(entstat.st_mode) && dflag)
		){	
			if(strcmp(namelist[n]->d_name,"..") && strcmp(namelist[n]->d_name,".")){
			printf("%s",path);
			if(strcmp(path,"./")){
				printf("/",path);
			}
				printf("%s\n", namelist[n]->d_name);
			}
		}
		free(namelist[n]);
	}
	free(namelist);	
	printf("\n");
	return;
}

int main(int argc, char** argv){

	//error checker
	int flag = 0;
	
	//for getopt
	char opt;
	
	struct stat entstat;	
	struct dirent **namelist;
	int n;
	char* path;

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
	
	if((fflag==0) && (dflag==0) && (lflag==0)){
		fflag = 1;
		dflag = 1;
		lflag = 1;
	}
	
	if(optind < argc){
		if(stat(argv[optind], &entstat)==0 && S_ISDIR(entstat.st_mode)){
			(void)actually_print_dir(argv[optind],fflag,dflag,lflag,sflag);
		}else{
			(void)actually_print_dir(DEFAULT_DIR,fflag,dflag,lflag,sflag);
		}
	}else{
		(void)actually_print_dir(DEFAULT_DIR,fflag,dflag,lflag,sflag);
	}

	return 0;
}
