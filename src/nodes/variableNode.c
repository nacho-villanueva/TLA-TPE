#include <stdio.h>
#include "variableNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"
#include "../adt/variable.h"


int parseVariableCreationNode(Node * node, U3D_Context *  context) {
    int ret = 0;
    union VariableValue value;

    logInfo("About to add a new variable to the list of variables. New variable: [type=%s]\n", getNodeTypeByCode(node->type));

    switch (node->type) {

        case INTEGER_VARIABLE_CREATION_NODE:
            value.integer = node->children[1]->value.integer;
            ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, value);
            if(ret == -1)
                return ret;
            parse("int %s = %d;", node->children[0]->value.string, node->children[1]->value.integer);
            break;
            
        case STRING_VARIABLE_CREATION_NODE:
            value.string = node->children[1]->value.string;
            ret = newVariable(node->children[0]->value.string, VARIABLE_STRING, value);
            if(ret == -1)
                return ret;
            parse("String %s = \"%s\";", node->children[0]->value.string, node->children[1]->value.string);
            break;

        case FLOAT_VARIABLE_CREATION_NODE:
            value.decimal = node->children[1]->value.decimal;
            ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, value);
            if(ret == -1)
                return ret;
            parse("float %s = %f;", node->children[0]->value.string, node->children[1]->value.decimal);
            break;

        case BOOLEAN_VARIABLE_CREATION_NODE:
            value.boolean = node->children[1]->value.boolean;
            ret = newVariable(node->children[0]->value.string, VARIABLE_BOOLEAN, value);
            if(ret == -1)
                return ret;
            if(node->children[1]->value.boolean)
                parse("boolean %s = true;", node->children[0]->value.string);
            else
                parse("boolean %s = false;", node->children[0]->value.string);
            break;

        default:
            break;
    }
    
    return ret;
}