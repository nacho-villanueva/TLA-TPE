#include "rootNodes.h"
#include <stdio.h>
#include "../utils/parser.h"

int parseBlock(Node * node, U3D_Context *  context, NodeType type){
    for(int i = 0; i < node -> childrenCount; i++){
        if(node -> children[i] -> type == type){
            return parseNode(node -> children[i], context);
        }
    }
    return 0;
}

int parseRootNode(Node * node, U3D_Context *  context){
    int ret = 0;
    // Parse in order and only parses each block once, despite multiple declarations
    ret += parseBlock(node, context, DEFINITIONS_NODE);
    ret += parseBlock(node, context, SETTINGS_NODE);
    ret += parseBlock(node, context, DRAW_NODE);

    return ret;
}

int parseDefinitionsNode(Node * node, U3D_Context *  context){
    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i], context);
    }
    return ret;
}

int parseSettingsNode(Node * node, U3D_Context * context){
    parse("\nvoid setup() { \n");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret = parseNode(node->children[i], context);
        if(ret < 0)
            return ret;
    }

    parse("\n}\n");
    return ret;
}

int parseDrawNode(Node * node, U3D_Context * context){
    parse("\nvoid draw() { \n");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret = parseNode(node->children[i], context);
        if(ret < 0)
            return ret;
    }

    parse("\n}\n");
    return ret;
}