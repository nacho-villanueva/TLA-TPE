#include <stdio.h>
#include "variableNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"
#include "../adt/variable.h"


int parseVariableCreationNode(Node * node, U3D_Context *  context) {
    int ret;
    union VariableValue value;

    logInfo("About to add a new variable to the list of variables. New variable: [type=%s]\n", getNodeTypeByCode(node->type));

    switch (node->type) {

        case INTEGER_VARIABLE_CREATION_NODE:
            value.integer = node->children[1]->value.integer;
            ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, value, context);
            if(ret == -1) 
                return ret;
            parse("int %s = %d;", node->children[0]->value.string, node->children[1]->value.integer);
            break;
            
        case STRING_VARIABLE_CREATION_NODE:
            value.string = node->children[1]->value.string;
            ret = newVariable(node->children[0]->value.string, VARIABLE_STRING, value, context);
            if(ret == -1)
                return ret;
            parse("String %s = \"%s\";", node->children[0]->value.string, node->children[1]->value.string);
            break;

        case FLOAT_VARIABLE_CREATION_NODE:
            value.decimal = node->children[1]->value.decimal;
            ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, value, context);
            if(ret == -1)
                return ret;
            parse("float %s = %f;", node->children[0]->value.string, node->children[1]->value.decimal);
            break;

        case BOOLEAN_VARIABLE_CREATION_NODE:
            value.boolean = node->children[1]->value.boolean;
            ret = newVariable(node->children[0]->value.string, VARIABLE_BOOLEAN, value, context);
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

int parseVariableUpdateNode(Node * node, U3D_Context *  context) {
    int ret = 0;

    if(node->childrenCount != 2) {
        logError(FATAL_ERROR, "Update variable node should have 2 children!\n");
        return -1;
    }

    if(node->children[0]->type != IDENTIFIER_NODE) {
        logError(FATAL_ERROR, "Identifier expected in first child of variable update node\n");
        return -1;
    }

    if(checkIfIdentifierIsUsed(node->children[0]->value.string, context->first) == 0) {
        logError(SYNTAX_ERROR, "Variable \"%s\" doesn't exist\n", node->children[0]->value.string);
        return -1;
    } 

    union VariableValue value;

    switch (node->type) {
        case NUMERIC_VARIABLE_UPDATE_NODE:
            if(node->children[1]->type == FLOAT_CONSTANT_NODE) {
                parse("%s = %f;\n", node->children[0]->value.string, node->children[1]->value.decimal);
                value.decimal = node->children[1]->value.decimal;
                return setVariableValue(node->children[0]->value.string, VARIABLE_FLOAT, value, context->first);
            } else if(node->children[1]->type == INTEGER_CONSTANT_NODE) {
                parse("%s = %d;\n", node->children[0]->value.string, node->children[1]->value.integer);
                value.integer = node->children[1]->value.integer;
                return setVariableValue(node->children[0]->value.string, VARIABLE_INTEGER, value, context->first);
            } else {
                logError(FATAL_ERROR, "impossible child of NUMERIC_VARIABLE_UPDATE_NODE\n");
                return -1;
            }
            
        case STRING_VARIABLE_UPDATE_NODE:  // a = "hola";
            if(node->children[1]->type != STRING_CONSTANT_NODE) {
                logError(FATAL_ERROR, "impossible child of STRING_VARIABLE_UPDATE_NODE\n");
                return -1;
            }
            parse("%s = \"%s\";\n", node->children[0]->value.string, node->children[1]->value.string);
            value.string = node->children[1]->value.string;
            return setVariableValue(node->children[0]->value.string, VARIABLE_STRING, value, context->first);    

        case BOOLEAN_VARIABLE_UPDATE_NODE:  // a = true;
            if(node->children[1]->type != BOOLEAN_CONSTANT_NODE) {
                logError(FATAL_ERROR, "impossible child of BOOLEAN_VARIABLE_UPDATE_NODE\n");
                return -1;
            }

            if(node->children[1]->value.boolean)
                parse("%s = true;\n", node->children[0]->value.string);
            else
                parse("%s = false;\n", node->children[0]->value.string);

            value.boolean = node->children[1]->value.boolean;
            return setVariableValue(node->children[0]->value.string, VARIABLE_BOOLEAN, value, context->first);    

        case IDENTIFIER_VARIABLE_UPDATE_NODE: // a = b;
            if(node->children[1]->type != IDENTIFIER_NODE) {
                logError(FATAL_ERROR, "impossible child of IDENTIFIER_VARIABLE_UPDATE_NODE\n");
                return -1;
            }

            enum VariableType ta = getVariableType(node->children[0]->value.string, context->first);
            enum VariableType tb = getVariableType(node->children[1]->value.string, context->first);

            union VariableValue va = getVariableValue(node->children[0]->value.string, context->first);
            union VariableValue vb = getVariableValue(node->children[1]->value.string, context->first);

            parse("%s = %s;\n", node->children[0]->value.string, node->children[1]->value.string);

            switch(tb) {
                case VARIABLE_BOOLEAN:
                    if(ta == VARIABLE_BOOLEAN) 
                        return setVariableValue(node->children[0]->value.string, VARIABLE_BOOLEAN, vb, context->first);
                    else {
                        logError(SYNTAX_ERROR, "Cannot assign value to a variable of different data type.\n");
                        return -1;
                    }
                case VARIABLE_FLOAT:
                    if(ta == VARIABLE_FLOAT) 
                        return setVariableValue(node->children[0]->value.string, VARIABLE_FLOAT, vb, context->first);
                    else {
                        logError(SYNTAX_ERROR, "Cannot assign value to a variable of different data type.\n");
                        return -1;
                    }
                case VARIABLE_STRING:
                    if(ta == VARIABLE_STRING) 
                        return setVariableValue(node->children[0]->value.string, VARIABLE_STRING, vb, context->first);
                    else {
                        logError(SYNTAX_ERROR, "Cannot assign value to a variable of different data type.\n");
                        return -1;
                    }
                case VARIABLE_INTEGER:
                    if(ta == VARIABLE_INTEGER) 
                        return setVariableValue(node->children[0]->value.string, VARIABLE_INTEGER, vb, context->first);
                    else {
                        logError(SYNTAX_ERROR, "Cannot assign value to a variable of different data type.\n");
                        return -1;
                    }
                default:
                    logError(FATAL_ERROR, "variable type does not exist\n");
                    return -1;
            }
            
            break;

        default:
            break;
    }
    
    logError(FATAL_ERROR, "reached end of parseUpdateVariable\n");
    return -1;
}