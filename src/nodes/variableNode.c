#include "variableNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

static Variable v, a, b;

int parseVariableCreationNode(Node * node, U3D_Context *  context, bool isConstant) {
    int ret = 0;
    int type;

    logInfo("About to add a new variable to the list of variables. New variable: [type=%s]\n", getNodeTypeByCode(node->type));

    switch (node->type) {

        case INTEGER_CONSTANT_CREATION_NODE:
        case INTEGER_VARIABLE_CREATION_NODE:
            if(node->childrenCount == 2) {
                type = node->children[1]->type; 
                if(type != IDENTIFIER_NODE) { // int x = 5 + y
                    if (type == PLUS_NODE || type == MINUS_NODE || type == TIMES_NODE || type == DIVIDE_NODE ||
                        type == MODULE_NODE) {
                        ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, context, isConstant, true);
                        if (ret < 0)
                            return ret;
                        if(isConstant)
                            parse("final ");
                        parse("int %s = ", node->children[0]->value.string);
                        ret = parseNode(node->children[1], context);
                        if (ret < 0)
                            return ret;
                        parse(";\n");
                    } else { // int x = 5;
                        ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, context, isConstant, true);
                        if (ret < 0)
                            return ret;
                        if(isConstant)
                            parse("final ");   
                        parse("int %s = %d;\n", node->children[0]->value.string, node->children[1]->value.integer);
                    }
                } else {  // int a = b;
                    v = getVariable(node->children[1]->value.string, context->first);

                    if(v == NULL) {
                        logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[1]->value.string);
                        return -1;
                    }
                    if(v->type != VARIABLE_INTEGER) {
                        logError(SYNTAX_ERROR, "Cannot asign a variable of different type\n");
                        return -1;
                    }

                    if(! v->isInitialized) {
                        logError(SYNTAX_ERROR, "Variable \"%s\" is not initialized\n", node->children[1]->value.string);
                        return -1;
                    }

                    ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, context, isConstant, true);
                    if (ret < 0)
                        return ret;
                    if(isConstant)
                        parse("final ");
                    parse("int %s = %s;\n", node->children[0]->value.string, node->children[1]->value.string);
                }
            } else {
                ret = newVariable(node->children[0]->value.string, VARIABLE_INTEGER, context, isConstant, false);
                if(ret < 0) 
                    return ret;
                parse("int %s;\n", node->children[0]->value.string);
            }
            break;
        case STRING_CONSTANT_CREATION_NODE:  
        case STRING_VARIABLE_CREATION_NODE:
            if(node->childrenCount == 2) {
                if(node->children[1]->type != IDENTIFIER_NODE) {
                    ret = newVariable(node->children[0]->value.string, VARIABLE_STRING, context, isConstant, true);
                    if(ret < 0)
                        return ret;
                    if(isConstant)
                        parse("final ");
                    parse("String %s = \"%s\";\n", node->children[0]->value.string, node->children[1]->value.string);
                } else {
                    v = getVariable(node->children[1]->value.string, context->first);

                    if(v == NULL) {
                        logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[1]->value.string);
                        return -1;
                    }
                    if(v->type != VARIABLE_STRING) {
                        logError(SYNTAX_ERROR, "Cannot asign a variable of different type\n");
                        return -1;
                    }

                    if(! v->isInitialized) {
                        logError(SYNTAX_ERROR, "Variable \"%s\" is not initialized\n", node->children[1]->value.string);
                        return -1;
                    }

                    ret = newVariable(node->children[0]->value.string, VARIABLE_STRING, context, isConstant, true);
                    if (ret < 0)
                        return ret;
                    if(isConstant)
                        parse("final ");
                    parse("String %s = %s;\n", node->children[0]->value.string, node->children[1]->value.string);
                }
            } else {
                ret = newVariable(node->children[0]->value.string, VARIABLE_STRING, context, isConstant, false);
                if(ret < 0)
                    return ret;
                parse("String %s;\n", node->children[0]->value.string);
            }
            break;
        case FLOAT_CONSTANT_CREATION_NODE:  
        case FLOAT_VARIABLE_CREATION_NODE:
            if(node->childrenCount == 2) {
                type = node->children[1]->type;
                if(type != IDENTIFIER_NODE) {
                    if(type == PLUS_NODE || type == MINUS_NODE || type == TIMES_NODE || type == DIVIDE_NODE || type == MODULE_NODE) {
                        ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, context, isConstant, true);
                        if(ret < 0) 
                            return ret;
                        if(isConstant)
                            parse("final ");
                        parse("float %s = ", node->children[0]->value.string);
                        ret = parseNode(node->children[1], context);
                        if(ret < 0) 
                            return ret;
                        parse(";\n");
                    } else {
                        ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, context, isConstant, true);
                        if(ret < 0)
                            return ret;
                        if(isConstant)
                            parse("final ");
                        if(node->children[1]->type == FLOAT_CONSTANT_NODE)
                            parse("float %s = %f;\n", node->children[0]->value.string, node->children[1]->value.decimal);
                        else
                            parse("float %s = %d;\n", node->children[0]->value.string, node->children[1]->value.integer);
                    }
                } else {
                    v = getVariable(node->children[1]->value.string, context->first);

                    if(v == NULL) {
                        logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[1]->value.string);
                        return -1;
                    }
                    if(v->type != VARIABLE_FLOAT) {
                        logError(SYNTAX_ERROR, "Cannot asign a variable of different type\n");
                        return -1;
                    }

                    if(! v->isInitialized) {
                        logError(SYNTAX_ERROR, "Variable \"%s\" is not initialized\n", node->children[1]->value.string);
                        return -1;
                    }
                    
                    ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, context, isConstant, true);
                    if (ret < 0)
                        return ret;
                    if(isConstant)
                        parse("final ");
                    parse("float %s = %s;\n", node->children[0]->value.string, node->children[1]->value.string);
                }   
            } else {
                ret = newVariable(node->children[0]->value.string, VARIABLE_FLOAT, context, isConstant, false);
                if(ret < 0)
                    return ret;
                parse("float %s;\n", node->children[0]->value.string);
            }
            break;

        // TODO: mejorar esto en el caso de que soportemos boolean
        case BOOLEAN_CONSTANT_CREATION_NODE:  
        case BOOLEAN_VARIABLE_CREATION_NODE:
            if(node->childrenCount == 2) {
                ret = newVariable(node->children[0]->value.string, VARIABLE_BOOLEAN, context, isConstant, true);
                if(ret < 0)
                    return ret;
                if(isConstant)
                    parse("final ");
                if(node->children[1]->value.boolean)
                    parse("boolean %s = true;\n", node->children[0]->value.string);
                else
                    parse("boolean %s = false;\n", node->children[0]->value.string);
            } else {
                ret = newVariable(node->children[0]->value.string, VARIABLE_BOOLEAN, context, isConstant, false);
                if(ret < 0)
                    return ret;
                parse("boolean %s;\n", node->children[0]->value.string);
            }
            break;
        default:
            logDebug("Impossible case in parseVariableCreationNode\n");
            return -1;
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

    Variable a = getVariable(node->children[0]->value.string, context->first);

    if(a == NULL) {
        logError(SYNTAX_ERROR, "Variable \"%s\" doesn't exist\n", node->children[0]->value.string);
        return -1;
    } 

    if(a->isConstant){
        logError(SYNTAX_ERROR, "Cannot modify variable \"%s\" because it is a constant\n", node->children[0]->value.string);
        return -1;
    }

    int type = node->children[1]->type;

    switch (node->type) {
        case NUMERIC_VARIABLE_UPDATE_NODE: // x = 5;
            if(type == FLOAT_CONSTANT_NODE) {
                v = getVariable(node->children[0]->value.string, context->first);
                if(v == NULL) {
                    logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                    return -1;
                }
                if(v->type != VARIABLE_FLOAT) {
                    logError(SYNTAX_ERROR, "Wrong type update on variable \"%s\". Float type expected.\n", node->children[0]->value.string);
                    return -1;
                }
                v->isInitialized = true;
                parse("%s = %f;\n", node->children[0]->value.string, node->children[1]->value.decimal);
                return 0;
            } else if(type == INTEGER_CONSTANT_NODE) {
                v = getVariable(node->children[0]->value.string, context->first);
                if(v == NULL) {
                    logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                    return -1;
                }
                if(v->type != VARIABLE_INTEGER) {
                    logError(SYNTAX_ERROR, "Wrong type update on variable \"%s\". Integer type expected.\n", node->children[0]->value.string);
                    return -1;
                }
                v->isInitialized = true;
                parse("%s = %d;\n", node->children[0]->value.string, node->children[1]->value.integer);
                return 0;            
            } else if (type == PLUS_NODE || type == MINUS_NODE || type == TIMES_NODE || type == DIVIDE_NODE || type == MODULE_NODE) {
                v = getVariable(node->children[0]->value.string, context->first);
                if(v == NULL) {
                    logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                    return -1;
                }
                v->isInitialized = true;
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
            v = getVariable(node->children[0]->value.string, context->first);
            if(v == NULL) {
                logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                return -1;
            }
            if(v->type != VARIABLE_STRING) {
                logError(SYNTAX_ERROR, "Wrong type update on variable \"%s\". String type expected.\n", node->children[0]->value.string);
                return -1;
            }
            v->isInitialized = true;
            parse("%s = \"%s\";\n", node->children[0]->value.string, node->children[1]->value.string);
            return 0;

        case BOOLEAN_VARIABLE_UPDATE_NODE:  // a = true;
            if(node->children[1]->type != BOOLEAN_CONSTANT_NODE) {
                logDebug("impossible child of BOOLEAN_VARIABLE_UPDATE_NODE\n");
                return -1;
            }
            v = getVariable(node->children[0]->value.string, context->first);
            if(v == NULL) {
                logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                return -1;
            }
            if(v->type != VARIABLE_BOOLEAN) {
                logError(SYNTAX_ERROR, "Wrong type update on variable \"%s\". Boolean type expected.\n", node->children[0]->value.string);
                return -1;
            }

            v->isInitialized = true;

            if(node->children[1]->value.boolean)
                parse("%s = true;\n", node->children[0]->value.string);
            else
                parse("%s = false;\n", node->children[0]->value.string);

            return 0;

        case IDENTIFIER_VARIABLE_UPDATE_NODE: // a = b;

            for(int i = 0; i < node->childrenCount; i++) {
                if(node->children[i]->type != IDENTIFIER_NODE) {
                    logDebug("impossible child of IDENTIFIER_VARIABLE_UPDATE_NODE\n");
                    return -1;
                }
            }

            a = getVariable(node->children[0]->value.string, context->first);
            if(a == NULL) {
                logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[0]->value.string);
                return -1;
            }

            b = getVariable(node->children[1]->value.string, context->first);
            if(b == NULL) {
                logError(SYNTAX_ERROR, "Variable \"%s\" does not exist\n", node->children[1]->value.string);
                return -1;
            }

            if(! b->isInitialized) {
                logError(SYNTAX_ERROR, "Variable \"%s\" is not initialized\n", node->children[1]->value.string);
                return -1;
            }

            enum VariableType ta = a->type;
            enum VariableType tb = b->type;

            a->isInitialized = true;

            parse("%s = %s;\n", node->children[0]->value.string, node->children[1]->value.string);

            switch(tb) {
                case VARIABLE_BOOLEAN:
                    if(ta != VARIABLE_BOOLEAN) {
                        logError(SYNTAX_ERROR, "Cannot assign value to a variable of different data type.\n");
                        return -1;
                    }
                    return 0;
                    
                case VARIABLE_FLOAT:
                    if(ta != VARIABLE_FLOAT) {
                        logError(SYNTAX_ERROR, "Cannot assign value to a variable of different data type.\n");
                        return -1;
                    }
                    return 0;

                case VARIABLE_STRING:
                    if(ta != VARIABLE_STRING) {
                        logError(SYNTAX_ERROR, "Cannot assign value to a variable of different data type.\n");
                        return -1;
                    }
                    return 0;

                case VARIABLE_INTEGER:
                    if(ta != VARIABLE_INTEGER) {
                        logError(SYNTAX_ERROR, "Cannot assign value to a variable of different data type.\n");
                        return -1;
                    }
                    return 0;

                default:
                    logDebug("variable type does not exist\n");
                    return -1;
            }
            
            return 0;
            
        default:
            break;
    }
    
    logDebug("reached end of parseUpdateVariable\n");
    return -1;
}
