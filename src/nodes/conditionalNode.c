#include "conditionalNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int parseOrAndConditionalNode(Node * node, U3D_Context *  context) {
    int ret;

    if(node->childrenCount != 2){
        logDebug("Expected 2 children in parseOrAndConditionalNode\n");
        return -1;
    }

    // Hay que asegurarnos que el 1er hijo sea algun tipo de condicional especifico
    if(
       node -> children[0] -> type != AND_NODE && 
       node -> children[0] -> type != OR_NODE && 
       node -> children[0] -> type != LT_NUMERIC_NODE && 
       node -> children[0] -> type != GT_NUMERIC_NODE && 
       node -> children[0] -> type != LE_NUMERIC_NODE && 
       node -> children[0] -> type != EQ_NUMERIC_NODE && 
       node -> children[0] -> type != NEQ_NUMERIC_NODE && 
       node -> children[0] -> type != EQ_STRING_NODE && 
       node -> children[0] -> type != NEQ_STRING_NODE && 
       node -> children[0] -> type != EQ_BOOLEAN_NODE && 
       node -> children[0] -> type != NEQ_BOOLEAN_NODE &&
       node -> children[0] -> type != BOOLEAN_CONSTANT_NODE &&
       node -> children[0] -> type != LT_IDENTIFIER_NODE &&
       node -> children[0] -> type != GT_IDENTIFIER_NODE &&
       node -> children[0] -> type != LE_IDENTIFIER_NODE &&
       node -> children[0] -> type != GE_IDENTIFIER_NODE &&
       node -> children[0] -> type != EQ_IDENTIFIER_NODE &&
       node -> children[0] -> type != NEQ_IDENTIFIER_NODE
       ){
        logDebug("Some type of conditional node expected in parseOrAndConditionalNode (part 1)\n");
        return -1;
    }

    
    ret = parseNode(node -> children[0], context);
    if(ret < 0)
        return ret;

    if(node->type == AND_NODE) {
        parse(" && ");
    } else if(node->type == OR_NODE) {
        parse(" || ");
    }

    // Hay que asegurarnos que el 2do hijo sea algun tipo de condicional especifico
    if(
       node -> children[0] -> type != AND_NODE && 
       node -> children[0] -> type != OR_NODE && 
       node -> children[0] -> type != LT_NUMERIC_NODE && 
       node -> children[0] -> type != GT_NUMERIC_NODE && 
       node -> children[0] -> type != LE_NUMERIC_NODE && 
       node -> children[0] -> type != EQ_NUMERIC_NODE && 
       node -> children[0] -> type != NEQ_NUMERIC_NODE && 
       node -> children[0] -> type != EQ_STRING_NODE && 
       node -> children[0] -> type != NEQ_STRING_NODE && 
       node -> children[0] -> type != EQ_BOOLEAN_NODE && 
       node -> children[0] -> type != NEQ_BOOLEAN_NODE &&
       node -> children[0] -> type != BOOLEAN_CONSTANT_NODE &&
       node -> children[0] -> type != LT_IDENTIFIER_NODE &&
       node -> children[0] -> type != GT_IDENTIFIER_NODE &&
       node -> children[0] -> type != LE_IDENTIFIER_NODE &&
       node -> children[0] -> type != GE_IDENTIFIER_NODE &&
       node -> children[0] -> type != EQ_IDENTIFIER_NODE &&
       node -> children[0] -> type != NEQ_IDENTIFIER_NODE
       ){
        logDebug("Some type of conditional node expected in parseOrAndConditionalNode (part 2)\n");
        return -1;
    }

    ret = parseNode(node -> children[1], context);
    if(ret < 0)
        return ret;

    return ret;
}

int parseNumericConditionalNode(Node * node, U3D_Context *  context) {
    int ret;

    if(node->childrenCount != 2){
        logDebug("Expected 2 children in parseNumericConditionalNode\n");
        return -1;
    }

    for(int i = 0; i < node->childrenCount; i++) {
        if(node->children[i]->type != INTEGER_CONSTANT_NODE && node->children[i]->type != FLOAT_CONSTANT_NODE 
                && node->children[i]->type != PLUS_NODE && node->children[i]->type != MINUS_NODE && node->children[i]->type != TIMES_NODE 
                && node->children[i]->type != DIVIDE_NODE && node->children[i]->type != MODULE_NODE && node->children[i]->type != IDENTIFIER_NODE) {
            logDebug("Numeric expression node expected in parseNumericConditionalNode\n");
            return -1;
        }
        if(node->children[i]->type == IDENTIFIER_NODE) {
            int ret2 = getVariableType(node->children[i]->value.string, context->first);
            if(ret2 != VARIABLE_FLOAT && ret2 != VARIABLE_INTEGER){
                logError(SYNTAX_ERROR, "Cannot make a numerical comparison if one of the 2 parts is not a number\n");
                return -1;
            }
        }

    }

    ret = parseNode(node -> children[0], context);
    if(ret < 0)
        return ret;

    switch(node->type){
        case LT_NUMERIC_NODE: parse(" < "); break;
        case GT_NUMERIC_NODE: parse(" > "); break;
        case LE_NUMERIC_NODE: parse(" <= "); break;
        case GE_NUMERIC_NODE: parse(" >= "); break;
        case EQ_NUMERIC_NODE: parse(" == "); break;
        case NEQ_NUMERIC_NODE: parse(" != "); break;
        default: 
            logDebug("Impossible conditionalNode state in parseNumericConditionalNode\n");
            return -1;
    }

    ret = parseNode(node -> children[1], context);
    if(ret < 0)
        return ret;

    return ret;
}

int parseStringConditionalNode(Node * node, U3D_Context *  context) {
    int ret;

    if(node->childrenCount != 2){
        logDebug("Expected 2 children in parseStringConditionalNode\n");
        return -1;
    }

    for(int i = 0; i < node->childrenCount; i++){
        if(node -> children[i] -> type != STRING_CONSTANT_NODE) {
            if(node -> children[i] -> type != IDENTIFIER_NODE) {
                logError(SYNTAX_ERROR, "Cannot make a string comparison if one of the 2 parts is not a string\n");
                return -1;
            }
            if(getVariableType(node -> children[i]->value.string, context->first) != VARIABLE_STRING){
                logError(SYNTAX_ERROR, "Cannot make a string comparison if one of the 2 parts is not a string\n");
                return -1;
            }
        }
    } 
    
    if(node->type == NEQ_STRING_NODE)
        parse("!");
    ret = parseNode(node -> children[0], context);
    if(ret < 0)
        return ret;
    parse(".equals(");

    ret = parseNode(node -> children[1], context);
    if(ret < 0)
        return ret;

    parse(")");

    return ret;
}

//TODO: si terminamos usando boolean, a esta funcion habría que pulirla tanto como pulimos todas las otras de este archivo
int parseBooleanConditionalNode(Node * node, U3D_Context *  context) {
    int ret;

    if(node->childrenCount != 2) {
        logDebug("Expected 2 children\n");
        return -1;
    }

    for(int i = 0; i < node->childrenCount; i++) {
        if(node -> children[i] -> type != BOOLEAN_CONSTANT_NODE){
            logError(SYNTAX_ERROR, "Cannot make a boolean comparison if one of the 2 parts is not a boolean\n");
            return -1;
        }
    }

    ret = parseNode(node -> children[0], context);
    if(ret < 0)
        return ret;

    if(node->type == EQ_BOOLEAN_NODE) {
        parse(" == ");
    } else if(node->type == NEQ_BOOLEAN_NODE) {
        parse(" != ");
    }

    ret = parseNode(node -> children[1], context);
    if(ret < 0)
        return ret;

    return ret;
}

int parseDoubleIdentifierConditionalNode(Node * node, U3D_Context *  context) {
    int ret = 0;

    if(node->childrenCount != 2){
        logDebug("Expected 2 children in parseDoubleIdentifierConditionalNode\n");
        return -1;
    }

    if(node->children[0]->type != IDENTIFIER_NODE && node->children[1]->type != IDENTIFIER_NODE) {
        logDebug("Expected 2 identifier nodes in parseDoubleIdentifierConditionalNode\n");
        return -1;
    }

    int firstVariableType = getVariableType(node->children[0]->value.string, context->first);
    int secondVariableType = getVariableType(node->children[1]->value.string, context->first);

    if(firstVariableType == -1){
        logError(SYNTAX_ERROR, "Variable \"%s\" doesn't exist\n", node->children[0]->value.string);
        return -1;
    }
    if(secondVariableType == -1){
        logError(SYNTAX_ERROR, "Variable \"%s\" doesn't exist\n", node->children[1]->value.string);
        return -1;
    }
    if(firstVariableType != secondVariableType){
        logError(SYNTAX_ERROR, "Cannot make logical operations between 2 variables of different type\n");
        return -1;
    }

    if(firstVariableType == VARIABLE_INTEGER || firstVariableType == VARIABLE_FLOAT) {

        ret = parseNode(node -> children[0], context);
        if(ret < 0)
            return ret;

        switch(node->type){
            case LT_IDENTIFIER_NODE: parse(" < "); break;
            case GT_IDENTIFIER_NODE: parse(" > "); break;
            case LE_IDENTIFIER_NODE: parse(" <= "); break;
            case GE_IDENTIFIER_NODE: parse(" >= "); break;
            case EQ_IDENTIFIER_NODE: parse(" == "); break;
            case NEQ_IDENTIFIER_NODE: parse(" != "); break;
            default: 
                logDebug("Impossible conditionalNode state in parseDoubleIdentifierConditionalNode\n");
                return -1;
        }

        ret = parseNode(node -> children[1], context);
        if(ret < 0)
            return ret;
    }
    else if(firstVariableType == VARIABLE_STRING) {
        if(node->type == NEQ_IDENTIFIER_NODE){
            parse("!");
        }
        ret = parseNode(node -> children[0], context);
        if(ret < 0)
            return ret;
        parse(".equals(");
        ret = parseNode(node -> children[1], context);
        if(ret < 0)
            return ret;
        parse(")");
    }

    return ret;
}