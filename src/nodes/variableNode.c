#include <stdio.h>
#include "variableNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"
#include "../adt/variable.h"


int parseVariableNode(Node * node, U3D_Context *  context) {
    int ret = 0;
    union VariableValue value;

    switch (node->type) {
    case INTEGER_VARIABLE_NODE:
        value.integer = node->children[1]->value.integer;
        newVariable(node->children[0]->value.string, VARIABLE_INTEGER, value);
        parse("int %s = %d;", node->children[0]->value.string, node->children[1]->value.integer);
        break;
    case STRING_VARIABLE_NODE:
        //newVariable(node->children[0]->value.string, VARIABLE_STRING, node->children[1]->value.string)
        parse("String %s = %s;", node->children[0]->value.string, node->children[1]->value.string);
        break;
    case FLOAT_VARIABLE_NODE:
        //newVariable(node->children[0]->value.string, VARIABLE_FLOAT, node->children[1]->value.decimal);
        parse("float %s = %f;", node->children[0]->value.string, node->children[1]->value.decimal);
        break;
    case BOOLEAN_VARIABLE_NODE:
        //newVariable(node->children[0]->value.string, VARIABLE_BOOLEAN, node->children[1]->value.boolean);
        if(node->children[1]->value.boolean) {
            parse("boolean %s = true;", node->children[0]->value.string);
        } else {
            parse("boolean %s = false;", node->children[0]->value.string);
        }
        break;
    default:
        break;
    }
    
    return ret;
}