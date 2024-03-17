#include "message.h"

struct Node{
    struct Message message;
    struct Node* next;
    struct Node* prev;
};

struct Node createNode(char* value, struct Node* next, struct Node* prev){
    struct Node node;
    node.next=next;
    node.prev=prev;
    node.message = createMessage(value);
    return node;
}

struct Node* deleteNode(struct Node* node){
    if(node==NULL){
        return NULL;
    }
    (void)deleteMessage(&((*node).message));
    return node->next;
}

void printNode(struct Node node){
    (void)printMessage(node.message);
    return;
}
void printNode(struct Node* node){
    if(node==NULL){printf("null\n");return;}
    (void)printMessage(node->message);
    return;
}
