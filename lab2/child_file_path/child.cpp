#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
//	printf("hello world!\n");
	printf("\tname=%s\n", argv[0]);
	printf("\tpid=%d\n", getpid());
	printf("\tppid=%d\n", getppid());
	printf("child exit\n");
	exit(EXIT_SUCCESS);
	//return 0;
}
