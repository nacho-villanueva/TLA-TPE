#include "node.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "logger.h"

Node* newNode(NodeType type, NodeValue value, int childrenCount, ...) {
    Node* node = malloc(NODE_SIZE);
    logDebug("Creating node %p (NodeType: %d)\n", node, type);
    node->type = type;
    node->value = value;
    node->childrenCount=childrenCount;
    node->children = malloc(NODE_SIZE * childrenCount);

    va_list valist;
    va_start(valist, childrenCount);
    for (size_t i = 0; i < childrenCount; i++)
        node->children[i] = va_arg(valist, Node*);
    va_end(valist);

    return node;
}

void addChildrenToNode(Node* node, int newChildrenCount, ...){
    node->children = realloc(node->children, NODE_SIZE * node->childrenCount + newChildrenCount);
    
    va_list valist;
    va_start(valist, newChildrenCount);
    for (size_t i = 0; i < newChildrenCount; i++){
        node->children[node->childrenCount + i] = va_arg(valist, Node*);
        logDebug("Adding child node %p (NodeType: %d) to node %p (NodeType: %d)\n",node->children[node->childrenCount + i], node->children[node->childrenCount + i] != NULL ? node->children[node->childrenCount + i]->type : -1, node, node->type);
    }
    va_end(valist);
    node->childrenCount += newChildrenCount;
}

void printTreeRec(Node * node, int step){
    for(int i = 0; i < step; i++){
        putchar(' ');
        putchar(' ');
        putchar(' ');
    }
    printf("|%s\n", NODE_NAMES[node->type]);
    if(node->childrenCount > 0){
        for(int i = 0; i < step; i++){
            putchar(' ');
            putchar(' ');
            putchar(' ');
        }
        printf("   \\________\n");
        for(int i = 0; i < node->childrenCount; i++){
            printTreeRec(node->children[i], step+1); 
        }
    }
}

void printTree(Node * node) {
    printTreeRec(node, 0);
}

