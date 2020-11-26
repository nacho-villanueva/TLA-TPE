#include "blockNode.h"
#include "node.h"
#include "../utils/parser.h"

int parseDefinitionsNode(Node * node){
    return 0;
}

int parseSettingsNode(Node * node){
    parse("\nvoid setup() { \n");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i]);
    }

    parse("\n}\n");
    return 0;
}

int parseDrawNode(Node * node){
    parse("\nvoid draw() { \n");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i]);
    }

    parse("\n}\n");
    return 0;
}

