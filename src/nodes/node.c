#include "node.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "../utils/logger.h"

#include "rootNode.h"
#include "blockNode.h"
#include "whileNode.h"

Node* newNode(NodeType type, NodeValue value, int childrenCount, ...) {
    Node* node = malloc(NODE_SIZE);
    logDebug("Creating node %p (NodeType: %s)\n", node, NODE_NAMES[type]);
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
        logDebug("Adding child node %p (NodeType: %s) to node %p (NodeType: %s)\n", node->children[node->childrenCount + i], node->children[node->childrenCount + i] != NULL ? NODE_NAMES[node->children[node->childrenCount + i]->type] : "NULL", node, NODE_NAMES[node->type]);
    }
    va_end(valist);
    node->childrenCount += newChildrenCount;
}

int parseNode(Node* node){
    if(node == NULL)
        return 0;
    switch (node->type)
    {
    case ROOT_NODE:
        return parseRootNode(node);
    case DRAW_NODE:
        return parseDrawNode(node);
    case WHILE_NODE:
        return parseWhileNode(node);
    case IF_NODE:
        return parseIfNode(node);
    default:
        logDebug("WARNING: Node parser not assigned (Type: %s)\n", NODE_NAMES[node->type]);
        return -1;
    }
}

void printTreeRec(Node * node, int step){
    for(int i = 0; i < step; i++){
        logInfo("   ");
    }
    if(node != NULL)
        logInfo("|%s\n", NODE_NAMES[node->type]);
    if(node->childrenCount > 0){
        for(int i = 0; i < step; i++){
            logInfo("   ");
        }
        logInfo("   \\________\n");
        for(int i = 0; i < node->childrenCount; i++){
            if(node->children[i])
                printTreeRec(node->children[i], step+1); 
        }
    }
}

void printTree(Node * node) {
    printTreeRec(node, 0);
}

