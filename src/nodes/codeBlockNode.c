#include "codeBlockNode.h"
#include <stdio.h>
#include "../utils/parser.h"
#include "../utils/logger.h"

int parseCodeBlockNode(Node * node, U3D_Context *  context){
    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        if(node->children[i] != NULL) {

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
                    /* separator */
                case NUMERIC_VARIABLE_UPDATE_NODE:
                case STRING_VARIABLE_UPDATE_NODE:
                case BOOLEAN_VARIABLE_UPDATE_NODE:
                case IDENTIFIER_VARIABLE_UPDATE_NODE:
                    return parseNode(node->children[0], context);
                default:
                    logError(SYNTAX_ERROR, "Impossible children type in CODE_BLOCK_NODE\n");
                    return -1;
            }
        }
    }
    return ret;
}