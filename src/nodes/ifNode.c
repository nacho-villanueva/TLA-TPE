#include <stdio.h>
#include "ifNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int parseIfNode(Node * node, U3D_Context *  context){
    int ret = 0;
    printf("Entre al IF_NODE\n");
    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "If node expects 2 children\n");
        return -1;
    }
    parse("if(");

    if(node -> children[0] -> type !=   /* TODO: HARDCODEADO CONDITIONAL_NODE */ INTEGER_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "Conditional node expected\n");
        return -1;
    }
    parse("5");
    // ret += parseNode(node -> children[0], context);

    parse(") {\n\t");
    if(node -> children[1] -> type != INTEGER_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "Code block noded expected\n");
        return -1;
    }
    // ret += parseNode(node -> children[1], context);
    parse("4");
    parse("}\n");

    return ret;
}