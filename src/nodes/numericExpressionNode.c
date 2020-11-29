#include "numericExpressionNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int parseNumericExpressionNode(Node * node, U3D_Context *  context) {

    int ret;
    Variable a;


    if(node->childrenCount != 1 && node->childrenCount != 2) {
        logDebug("Expected 1 or 2 children in numeric expression node\n");
        return -1;
    }

    for(int i = 0; i < node->childrenCount; i++) {
        if(node->children[i]->type != INTEGER_CONSTANT_NODE && node->children[i]->type != FLOAT_CONSTANT_NODE 
                && node->children[i]->type != PLUS_NODE && node->children[i]->type != MINUS_NODE && node->children[i]->type != TIMES_NODE 
                && node->children[i]->type != DIVIDE_NODE && node->children[i]->type != MODULE_NODE
                && node->children[i]->type != IDENTIFIER_NODE) {
            logDebug("Numeric expression node expected (in numericExpressionNode)\n");
            return -1;
        }

        if(node->children[i]->type == IDENTIFIER_NODE){
            a = getVariable(node->children[i]->value.string, context->first);

            if(a == NULL){
                logError(SYNTAX_ERROR, "Variable \"%s\" doesn't exist\n", node->children[i]->value.string);
                return -1;
            }

            if(a->type != VARIABLE_FLOAT && a->type != VARIABLE_INTEGER) {
                logError(SYNTAX_ERROR, "Non-numeric variable in numeric expression\n");
                return -1;
            }

            if(! a->isInitialized){
                logError(SYNTAX_ERROR, "Variable \"%s\" is not initialized\n", node->children[i]->value.string);
                return -1;
            }
        }
    }

    ret = parseNode(node -> children[0], context);
    if(ret < 0)
        return ret;

    if(node->childrenCount > 1) {

        switch(node->type){
            case PLUS_NODE: parse(" + "); break;
            case MINUS_NODE: parse(" - "); break;
            case TIMES_NODE: parse(" * "); break;
            case DIVIDE_NODE: parse(" / "); break;
            case MODULE_NODE: parse(" %% "); break;
            default: 
                logDebug("Impossible numeric expression state in parseNumericExpressionNode\n");
                return -1;
        }

        ret = parseNode(node -> children[1], context);
        if(ret < 0)
            return ret;
    }
    
    return ret;

}
