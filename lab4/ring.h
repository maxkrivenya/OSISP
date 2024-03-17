#include "node.h"

struct Ring{
    struct Node* head;
    struct Node* tail;
    int pushed;
    int pulled;
};

struct Ring createRing(){
    struct Ring ring = {NULL,NULL,0,0};
    return ring;
}

void deleteRing(struct Ring* ring){
    if((*ring).head==NULL){return;}
    if((*ring).head->next==(*ring).head){
        (void)deleteNode(ring->head);
        ring->head=NULL;
        ring->tail=NULL;
        return;
    }

    struct Node* temp = (*ring).head->next;
    while(temp != (*ring).head){

    }
    if((*ring).head->message.value != NULL){
        free((*ring).head->message.value);
    }
    (*ring).head=NULL;
    (*ring).tail=NULL;
    return;
}

void printRing(struct Ring ring){
    if(ring.head==NULL){printf("ring is empty!\n");}
    if(ring.head->next==ring.head){
        (void)printNode(ring.head);
        return;
    }

    struct Node* temp = ring.head->next;
    while(temp != ring.head && temp != NULL){
        temp = deleteNode(temp);
    }
    ring.head=NULL;
    ring.tail=NULL;

    if(temp==NULL){
        (void)printf("temp is null!\n");
        return;
    }
    (void)deleteNode(temp);
    return;
}

