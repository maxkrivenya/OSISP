#include <iterator>
#include <stdio.h>
#include <unistd.h>


int main (int argc, char *argv[], char *env[]) {
    for(int i = 0; environ[i] != NULL && i <= 10; i++){    
        printf("%s\n",environ[i]);
    }
    return 0;
}
