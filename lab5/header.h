#include "definitions.h"

//killing processes on loop-end
volatile int producers_killed[MAX_CHILD_AMT];
volatile int consumers_killed[MAX_CHILD_AMT];

//printing a message
void msgprint(struct message msg){
    int size = msg.content[3] & 0xFF;
    printf("\ttype:%c\n\thash:%c%c\n\tsize:%d\n\tdata:",msg.content[0], msg.content[1] , msg.content[2], size);
    for(int i = 0; i < size; i++){
        printf("%c", msg.content[i]);
    }
    printf(LINE_SEPARATOR);
}

//creating a message
struct message msg_create(){

    struct message msg;
    
    int sizeint = 0;
    char sizechar = '0';
    unsigned short word = 0;

    do{
        sizeint = rand()%257;
    }while(!sizeint);

    if(sizeint < 256){
        sizechar = sizeint;
    }

    for(int i = 4; i < 4 + sizeint; i++){
        msg.content[i] = 'A' + rand()%26;
        word = word + (msg.content[i] % 10);
    }

    msg.mtype = 1;
    msg.content[0] = '1';
    msg.content[1] = ((unsigned char*)(&word))[0];
    msg.content[2] = ((unsigned char*)(&word))[1];
    msg.content[3] = sizechar;
    msg.content[4 + sizeint] = '\0';

    return msg;
}
