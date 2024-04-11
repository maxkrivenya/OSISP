#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct message{
    char* content;
};

/*
 * content[0]                           = type
 * content[1->2]                        = hash
 * content[3]                           = size
 * content[4 -> (4 + ((size+3)/4)*4)]   = data
 *
 * */

void queue_stat(int msqid){
    int flag = 0;

    flag = msgctl(msqid, IPC_STAT, &buf);
    if(flag==-1){
        strerror(errno);
        exit(-1);
    }

    printf("\tpermissions: %d\n", buf.msg_perm.mode);
    printf("\tamt of msgs: %lu\n", buf.msg_qnum);
   
    /*
}
