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


//function to actually do the printing to avoid copying strings and complicated IFs in main()
void actually_print_dir(const char* path, bool fflag, bool dflag, bool lflag, bool sflag){
	//error checker
	int flag = 0;
	//to check for entry type
	struct stat entstat;	
	//to get the entries
	struct dirent **namelist;
	//to get amt of entries
	int n;

	//get list of entries (sflag?sorted:not)
	if(sflag){
		n = scandir(path, &namelist, 0, alphasort);
	}else{
		n = scandir(path, &namelist, 0, 0);
	}

	//if <0 entries, scandir failed
	if (n < 0){
		perror("scandir");
		return;
	}
	if (n <= 2){
		printf("DIR '%s' is empty\n",path);
		return;
	}
	//loop over dir entries
	while (--n) {
		//printf("iterator n=%d\n",n);
		char entry[POSIX_PATH_MAX] = {0};
		(void)strcat(entry,path);	
		(void)strcat(entry,"/");
		(void)strcat(entry,namelist[n]->d_name);	//entry == truepath but not laggy
		flag=lstat(entry,&entstat);			//get entry type
		if(flag==-1){					//if failed, error
			perror("stat failed");
			return;
		}		
		if((S_ISREG(entstat.st_mode) && fflag) || (S_ISLNK(entstat.st_mode) && lflag) ){	
			if(strcmp(namelist[n]->d_name,"..") && strcmp(namelist[n]->d_name,".")){	//if not '..' or '.'
			printf("%s",path);								//print path
				if(strcmp(path,"./")){					
					printf("/");								//if path isn't './' print '/'
				}
				printf("%s\n", namelist[n]->d_name);					//print filename
			}
		}
		if(S_ISDIR(entstat.st_mode) && dflag){
			if(namelist[n]->d_name[0] != '.'){	//if not '..' or '.'
				if(namelist[n]->d_name[0]=='.'){
					printf("%s/",path);
					printf("%s\n",namelist[n]->d_name[0]);
				}else{
					printf("\n=====================================================================%s=========================\n", entry);
					actually_print_dir(entry,fflag,dflag,lflag,sflag);
				}
			}
		}
		free(namelist[n]);									//free entry 
	}
	free(namelist);	
//	printf("\n");
	return;
}

int main(int argc, char** argv){

	//error checker
	int flag = 0;
	
	//for getopt
	char opt;

	//check if user-given path is valid
	struct stat entstat;	

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
	}while(opt!=-1);	//set flags according to opts
	
	if((fflag==0) && (dflag==0) && (lflag==0)){	//if none set, set all
		fflag = 1;
		dflag = 1;
		lflag = 1;
	}
	
	if(optind < argc){										//if given a path
		if(stat(argv[optind], &entstat)==0 && S_ISDIR(entstat.st_mode)){
			(void)actually_print_dir(argv[optind],fflag,dflag,lflag,sflag);				//if it's valid, use it
		}else{
			(void)actually_print_dir(DEFAULT_DIR,fflag,dflag,lflag,sflag);				//else use defaule
		}
	}else{
		(void)actually_print_dir(DEFAULT_DIR,fflag,dflag,lflag,sflag);				//else use defaule
	}

	return 0;
}
