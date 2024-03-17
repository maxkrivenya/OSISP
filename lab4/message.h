#include "header.h"

struct Message{
    char* value;
};

struct Message createMessage(char* value){
    struct Message message;
    
    message.value = (char*)calloc(sizeof(value),sizeof(char));
    (void)strcpy(message.value, value);

    return message;
}

void deleteMessage(struct Message* message){
    if(message==NULL){
        return;
    }
    if((*message).value==NULL){
        return;
    }
    (void)free((*message).value);
    return;
}

void printMessage(struct Message message){
    if(message.value == NULL){
        (void)printf("NULL\n");
        return;
    }
    (void)puts(message.value);
}

