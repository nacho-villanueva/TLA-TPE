#include <stdio.h>  // TODO: quizas se borre
#include "codeLineNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int parseCodeLineNode(Node * node, U3D_Context *  context) {
    int ret = 0;
    
    // TODO: ¿chequear por la cantidad de hijos?

    if(node->childrenCount == 0) {
        printf("CodeLineBlock tiene cero hijos.\n");
        return 0;
    }

    switch(node->children[0]->type){
        case IF_NODE:
        case WHILE_NODE:
            return parseNode(node->children[0], context);
        case INTEGER_CONSTANT_NODE:
            parse("%d",node->children[0]->value.integer);
            return 0;
        case FLOAT_CONSTANT_NODE:
            parse("%f",node->children[0]->value.decimal);
            return 0;
        case INTEGER_VARIABLE_CREATION_NODE:
        case STRING_VARIABLE_CREATION_NODE:
        case FLOAT_VARIABLE_CREATION_NODE:
        case BOOLEAN_VARIABLE_CREATION_NODE:
            return parseNode(node->children[0], context);
        default:
            logError(SYNTAX_ERROR, "Impossible children type in CODE_LINE_NODE\n");
            return -1;
    }

    return ret;
}
