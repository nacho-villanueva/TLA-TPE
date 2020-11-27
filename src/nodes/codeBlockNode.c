#include "codeBlockNode.h"
#include <stdio.h>
#include "../utils/parser.h"

int parseCodeBlockNode(Node * node, U3D_Context *  context){
    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        if(node->children[i] != NULL) {
            ret += parseNode(node->children[i], context);
        }
    }
    return ret;
}