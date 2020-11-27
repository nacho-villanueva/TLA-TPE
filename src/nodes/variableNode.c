#include <stdio.h>
#include "variableNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int parseVariableNode(Node * node, U3D_Context *  context) {
    int ret = 0;

    if(node->childrenCount != 0){
        logError(SYNTAX_ERROR, "Expected 0 children\n");
        return -1;
    }

    switch (node->type) {
    case INTEGER_VARIABLE_NODE:
        if(node->value.integer == 2){
            printf("HOLE\n");
        }
        parse("int %s = %d;", node->value.identifier, node->value.integer);
        break;
    case STRING_VARIABLE_NODE:
        parse("String %s = %s;", node->value.identifier, node->value.string);
        break;
    case FLOAT_VARIABLE_NODE:
        parse("float %s = %f;", node->value.identifier, node->value.decimal);
        break;
    case BOOLEAN_VARIABLE_NODE:
        if(node->value.boolean)
            parse("boolean %s = true;", node->value.identifier);
        else
            parse("boolean %s = false;", node->value.identifier);
        break;
    default:
        break;
    }
    
    return ret;
}