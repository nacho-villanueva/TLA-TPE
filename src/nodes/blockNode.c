#include "blockNode.h"
#include "../utils/parser.h"

int parseDefinitionsNode(Node * node, U3D_Context *  context){
    return 0;
}

int parseSettingsNode(Node * node, U3D_Context * context){
    parse("\nvoid setup() { \n");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i], context);
    }

    parse("\n}\n");
    return 0;
}

int parseDrawNode(Node * node, U3D_Context * context){
    parse("\nvoid draw() { \n");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i], context);
    }

    parse("\n}\n");
    return 0;
}

