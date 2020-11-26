#include "conditionalNode.h"
#include "../utils/logger.h"

int parseOrAndConditionalNode(Node * node) {
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "Expected 2 children\n");
        return -1;
    }

    if(node -> children[0] -> type != CONDITIONAL_NODE){
        logError(SYNTAX_ERROR, "Conditional node expected\n");
        return -1;
    }
    ret += parseNode(node -> children[0]);

    if(node->type == "AND_NODE") {
        parse(" && ");
        //TODO: chequear valor de retorno de la 1era parte. SI es false, no analizamos el 2do nodo
    } else if(node->type == "OR_TYPE") {
        parse(" || ");
        //TODO: chequear valor de retorno de la 1era parte. SI es true, no analizamos el 2do nodo
    }

    if(node -> children[1] -> type != CONDITIONAL_NODE){
        logError(SYNTAX_ERROR, "Conditional node expected\n");
        return -1;
    }

    ret += parseNode(node -> children[1]);

    return ret;
}

int parseNumericConditionalNode(Node * node) {
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "Expected 2 children\n");
        return -1;
    }

    if(node -> children[0] -> type != INTEGER_CONSTANT_NODE || node -> children[0] -> type != FLOAT_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "Integer constant node or float constant node expected\n");
        return -1;
    }
    ret += parseNode(node -> children[0]);

    switch(node->type){
        case LT_NUMERIC_NODE: parse(" < "); break;
        case GT_NUMERIC_NODE: parse(" > "); break;
        case LE_NUMERIC_NODE: parse(" <= "); break;
        case GE_NUMERIC_NODE: parse(" >= "); break;
        case EQ_NUMERIC_NODE: parse(" == "); break;
        case NEQ_NUMERIC_NODE: parse(" != "); break;
    }

    if(node -> children[0] -> type != INTEGER_CONSTANT_NODE || node -> children[0] -> type != FLOAT_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "Integer constant node or float constant node expected\n");
        return -1;
    }

    ret += parseNode(node -> children[1]);

    return ret;
}

int parseStringConditionalNode(Node * node) {
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "Expected 2 children\n");
        return -1;
    }

    if(node -> children[0] -> type != STRING_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "String constant node expected\n");
        return -1;
    }
    ret += parseNode(node -> children[0]);

    if(node->type == "EQ_STRING_NODE") {
        parse(" == ");
    } else if(node->type == "NEQ_STRING_NODE") {
        parse(" != ");
    }
    
    if(node -> children[1] -> type != STRING_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "String constant node expected\n");
        return -1;
    }

    ret += parseNode(node -> children[1]);

    return ret;
}

int parseBooleanConditionalNode(Node * node) {
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "Expected 2 children\n");
        return -1;
    }

    if(node -> children[0] -> type != BOOLEAN_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "Boolean constant node expected\n");
        return -1;
    }
    ret += parseNode(node -> children[0]);

    if(node->type == "EQ_BOOLEAN_NODE") {
        parse(" == ");
    } else if(node->type == "NEQ_BOOLEAN_NODE") {
        parse(" != ");
    }
    
    if(node -> children[1] -> type != BOOLEAN_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "Boolean constant node expected\n");
        return -1;
    }

    ret += parseNode(node -> children[1]);

    return ret;
}
