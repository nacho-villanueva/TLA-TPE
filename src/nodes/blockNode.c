#include "blockNode.h"
#include "node.h"
#include "../utils/parser.h"

int parseDefinitionsNode(Node * node){
    
}

int parseSettingsNode(Node * node){
}

int parseDrawNode(Node * node){
    parse("void draw() { ");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i]);
    }

    parse("}");
    return 0;
}

