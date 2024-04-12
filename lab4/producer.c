#include "header.h"

int main(int argc, char* argv[], char* envp[]){
    int flag  = 0;
    int msqid = 0;
    struct message msg;
    do{
//1. CREATE msg
        msg = msg_create();
    
//2. take mutex 
    
//3. wait for enough space ?
    
//4. PUT msg
        flag = msgsnd(msqid, &msg, sizeof(msg), 0);
        if(flag==-1){
            strerror(errno);
            exit(-1);
        }
    
//5. dec space

//6. give mutex

//7. if signal, exit(1)
    }while(killed == 0);
    exit(1);
}
