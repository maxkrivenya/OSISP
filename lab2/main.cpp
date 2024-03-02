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
#define CHILD_PATH "CHILD_PATH"

int main(int argc, char **argv, char **envp){
	char* child_path = getenv(CHILD_PATH);
	pid_t pid;
	int flag = 0;
	char get;
	char bufclear;
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
	}

	do{
		fflush(stdin);
		get = getc(stdin);
		getc(stdin);
		if(get == 'q'){
			puts("parent exit");
			exit(EXIT_SUCCESS);
		}
		pid = fork();

		switch (pid) {
			case -1:{
					perror("fork");
					exit(EXIT_FAILURE);
				}

			case 0:{
				       execve(child_path, argv, envp);
			       }

			default:{
					printf("child : %jd\n", (intmax_t) pid);
					wait(&flag);
					if (flag == -1) {
						perror("waitpid");
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
