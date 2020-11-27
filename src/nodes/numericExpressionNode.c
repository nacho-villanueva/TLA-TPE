#include <stdio.h>
#include "numericExpressionNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int parseNumericExpressionNode(Node * node, U3D_Context *  context) {

    int ret = 0;

    for(int i = 0; i < node->childrenCount; i++) {
        if(node->children[i]->type != INTEGER_CONSTANT_NODE && node->children[i]->type != FLOAT_CONSTANT_NODE 
                && node->children[i]->type != PLUS_NODE && node->children[i]->type != MINUS_NODE && node->children[i]->type != TIMES_NODE 
                && node->children[i]->type != DIVIDE_NODE && node->children[i]->type != MODULE_NODE) {
            logError(SYNTAX_ERROR, "Numeric expression node expected (in numericExpressionNode)\n");
            return -1;
        }
    }

    ret += parseNode(node -> children[0], context);

    if(node->childrenCount > 1) {

        switch(node->type){
            case PLUS_NODE: parse(" + "); break;
            case MINUS_NODE: parse(" - "); break;
            case TIMES_NODE: parse(" * "); break;
            case DIVIDE_NODE: parse(" / "); break;
            case MODULE_NODE: parse(" %% ");break;
            default: 
                // TODO: revisar este caso
                logError(FATAL_ERROR, "Impossible numeric expression state. Returning -1\n");
                return -1;
        }

        ret += parseNode(node -> children[1], context);
    }
    
    return ret;

}