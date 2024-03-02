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
#define CHILD_PATH "CHILD_PATH"
#define CHILD_NAME "child"
#define NAME_SIZE 10
int main(int argc, char **argv, char **envp){
	char* child_path = getenv(CHILD_PATH);
	if(child_path==NULL){
		printf("ur env var is fucked up\n");
		return 0;
	}
	pid_t pid;
	int flag = 0;
	char get;
	int counter = 0;

	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
	}

	char* name = (char*)calloc(NAME_SIZE,1);
	(void)strcat(name,  CHILD_NAME);

	do{
		fflush(stdin);
		get = getc(stdin);
		getc(stdin);
		if(get == 'q'){
			puts("parent exit");
			free(name);
			exit(EXIT_SUCCESS);
		}
		pid = fork();

		switch (pid) {
			case -1:{
					perror("fork");
					free(name);
					exit(EXIT_FAILURE);

				}

			case 0:{
				       name[5] = '0' + counter / 10;
				       name[6] = '0' + counter % 10;
				       argv[0] = name;

				       execve(child_path, argv, envp);
			       }

			default:{
					counter++;
					printf("child : %jd\n", (intmax_t) pid);
					wait(&flag);
					if (flag == -1) {
						perror("waitpid");
						free(name);
						exit(EXIT_FAILURE);
					}
				}
		}
	}while(get != 'q');

	/*
	   for (char **env = envp; *env != 0; env++)	{
	   char *thisEnv = *env;
	   printf("%s\n", thisEnv);    
	   }
	   */

	return 0;
}
