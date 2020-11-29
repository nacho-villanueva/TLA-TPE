#include "variableNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"


int parseVariableCreationNode(Node * node, U3D_Context *  context) {
    int ret = 0;
    union VariableValue value;
    int type;

    logInfo("About to add a new variable to the list of variables. New variable: [type=%s]\n", getNodeTypeByCode(node->type));

    switch (node->type) {

        case INTEGER_VARIABLE_CREATION_NODE:
            if(node->childrenCount == 2) {
                type = node->children[1]->type;
                if(type == PLUS_NODE || type == MINUS_NODE || type == TIMES_NODE || type == DIVIDE_NODE || type == MODULE_NODE) {
                    value.integer = 0;
                    ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, value, context);
                    if(ret < 0) 
                        return ret;
                    parse("int %s = ", node->children[0]->value.string);
                    ret = parseNode(node->children[1], context);
                    if(ret < 0) 
                        return ret;
                    parse(";\n");
                } else {
                    value.integer = node->children[1]->value.integer;
                    ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, value, context);
                    if(ret < 0) 
                        return ret;
                    parse("int %s = %d;\n", node->children[0]->value.string, node->children[1]->value.integer);
                }
            } else {
                value.integer = 0;
                ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, value, context);
                if(ret < 0) 
                    return ret;
                parse("int %s;\n", node->children[0]->value.string);
            }
            break;
            
        case STRING_VARIABLE_CREATION_NODE:
            if(node->childrenCount == 2) {
                value.string = node->children[1]->value.string;
                ret = newVariable(node->children[0]->value.string, VARIABLE_STRING, value, context);
                if(ret < 0)
                    return ret;
                parse("String %s = \"%s\";\n", node->children[0]->value.string, node->children[1]->value.string);
            } else {
                value.string = "";
                ret = newVariable(node->children[0]->value.string, VARIABLE_STRING, value, context);
                if(ret < 0)
                    return ret;
                parse("String %s;\n", node->children[0]->value.string);
            }
            break;

        case FLOAT_VARIABLE_CREATION_NODE:
            if(node->childrenCount == 2) {
                type = node->children[1]->type;
                if(type == PLUS_NODE || type == MINUS_NODE || type == TIMES_NODE || type == DIVIDE_NODE || type == MODULE_NODE) {
                    value.decimal = 0.0;
                    ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, value, context);
                    if(ret < 0) 
                        return ret;
                    parse("float %s = ", node->children[0]->value.string);
                    ret = parseNode(node->children[1], context);
                    if(ret < 0) 
                        return ret;
                    parse(";\n");
                } else {
                    value.decimal = 0.0;
                    
                    ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, value, context);
                    if(ret < 0)
                        return ret;

                    if(node->children[1]->type == FLOAT_CONSTANT_NODE)
                        parse("float %s = %f;\n", node->children[0]->value.string, node->children[1]->value.decimal);
                    else
                        parse("float %s = %d;\n", node->children[0]->value.string, node->children[1]->value.integer);
                }
            } else {
                value.decimal = 0.0;
                ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, value, context);
                if(ret < 0)
                    return ret;
                parse("float %s;\n", node->children[0]->value.string);
            }
            break;

        case BOOLEAN_VARIABLE_CREATION_NODE:
            if(node->childrenCount == 2) {
                value.boolean = node->children[1]->value.boolean;
                ret = newVariable(node->children[0]->value.string, VARIABLE_BOOLEAN, value, context);
                if(ret < 0)
                    return ret;
                if(node->children[1]->value.boolean)
                    parse("boolean %s = true;\n", node->children[0]->value.string);
                else
                    parse("boolean %s = false;\n", node->children[0]->value.string);
            } else {
                value.boolean = false;
                ret = newVariable(node->children[0]->value.string, VARIABLE_BOOLEAN, value, context);
                if(ret < 0)
                    return ret;
                parse("boolean %s;\n", node->children[0]->value.string);
            }

            break;

        default:
            break;
    }
    
    return ret;
}

int parseVariableUpdateNode(Node * node, U3D_Context *  context) {
    int ret;

    if(node->childrenCount != 2) {
        logDebug("Update variable node should have 2 children in parseVariableUpdateNode\n");
        return -1;
    }

    if(node->children[0]->type != IDENTIFIER_NODE) {
        logDebug("Identifier expected in first child of variable update node\n");
        return -1;
    }

    if(checkIfIdentifierIsUsed(node->children[0]->value.string, context->first) == 0) {
        logError(SYNTAX_ERROR, "Variable \"%s\" doesn't exist\n", node->children[0]->value.string);
        return -1;
    } 

    int type = node->children[1]->type;
    union VariableValue value;

    switch (node->type) {
        case NUMERIC_VARIABLE_UPDATE_NODE: // x = 5;
            if(type == FLOAT_CONSTANT_NODE) {
                parse("%s = %f;\n", node->children[0]->value.string, node->children[1]->value.decimal);
                if(checkIfIdentifierIsUsed(node->children[0]->value.string, context->first) == 0) {
                    logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                    return -1;
                }
                return 0;
            } else if(type == INTEGER_CONSTANT_NODE) {
                parse("%s = %d;\n", node->children[0]->value.string, node->children[1]->value.integer);
                if(checkIfIdentifierIsUsed(node->children[0]->value.string, context->first) == 0) {
                    logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                    return -1;
                }
                return 0;            
            } else if (type == PLUS_NODE || type == MINUS_NODE || type == TIMES_NODE || type == DIVIDE_NODE || type == MODULE_NODE) {
                parse("%s = ", node->children[0]->value.string);
                ret = parseNode(node->children[1], context);
                if(ret < 0)
                    return -1;
                parse(";\n");
                return 0;            
            } else{
                logDebug("impossible child of NUMERIC_VARIABLE_UPDATE_NODE\n");
                return -1;
            }
            
        case STRING_VARIABLE_UPDATE_NODE:  // a = "hola";
            if(node->children[1]->type != STRING_CONSTANT_NODE) {
                logDebug("impossible child of STRING_VARIABLE_UPDATE_NODE\n");
                return -1;
            }
            if(checkIfIdentifierIsUsed(node->children[0]->value.string, context->first) == 0) {
                logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                return -1;
            }
            parse("%s = \"%s\";\n", node->children[0]->value.string, node->children[1]->value.string);
            value.string = node->children[1]->value.string;
            ret = setVariableValue(node->children[0]->value.string, VARIABLE_STRING, value, context->first);
            if(ret < 0) {
                logError(SYNTAX_ERROR, "Wrong type update on variable \"%s\"\n", node->children[0]->value.string);
                return ret;
            }
            break;

        case BOOLEAN_VARIABLE_UPDATE_NODE:  // a = true;
            if(node->children[1]->type != BOOLEAN_CONSTANT_NODE) {
                logDebug("impossible child of BOOLEAN_VARIABLE_UPDATE_NODE\n");
                return -1;
            }
            if(checkIfIdentifierIsUsed(node->children[0]->value.string, context->first) == 0) {
                logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                return -1;
            }

            if(node->children[1]->value.boolean)
                parse("%s = true;\n", node->children[0]->value.string);
            else
                parse("%s = false;\n", node->children[0]->value.string);

            value.boolean = node->children[1]->value.boolean;
            ret = setVariableValue(node->children[0]->value.string, VARIABLE_BOOLEAN, value, context->first);
            if(ret < 0) {
                logError(SYNTAX_ERROR, "Wrong type update on variable \"%s\"\n", node->children[0]->value.string);
                return ret;
            }
            break;

        case IDENTIFIER_VARIABLE_UPDATE_NODE: // a = b;

            for(int i = 0; i < node->childrenCount; i++) {
                if(node->children[i]->type != IDENTIFIER_NODE) {
                    logDebug("impossible child of IDENTIFIER_VARIABLE_UPDATE_NODE\n");
                    return -1;
                }
                if(checkIfIdentifierIsUsed(node->children[i]->value.string, context->first) == 0) {
                    logError(SYNTAX_ERROR, "variable \"%s\" does not exist\n", node->children[i]->value.string);
                    return -1;
                }
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
                    logDebug("variable type does not exist\n");
                    return -1;
            }

        default:
            break;
    }
    
    logDebug("reached end of parseUpdateVariable\n");
    return -1;
}
