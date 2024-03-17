#include "header.h"
#include "ring.h"

int main(){
    struct Node node = createNode("hello wrld", NULL,NULL);
    (void)printNode(node);
    (void)deleteNode(&node);
    (void)printNode(node);
    return 0;
}
