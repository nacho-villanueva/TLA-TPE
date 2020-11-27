#include <stdlib.h>
#include <malloc.h>
#include <strings.h>
#include "figureNode.h"
#include "../utils/logger.h"

void addFigureToTable(Figure figure, U3D_Context * context) {
    context->figuresCount++;
    context->figuresTable = reallocarray(context->figuresTable, context->figuresCount, sizeof(Figure));
    context->figuresTable[context->figuresCount-1] = figure;
}

Node * getFigureAttributeValueNode(Node * node, char * attr){
    if(node->children[0]->type == FIGURE_ATTRIBUTE_LIST_NODE){
        Node * attrList = node->children[0];
        for(int i = 0; i < attrList->childrenCount; i++){
            Node * identifier = getChildNode(attrList->children[i], IDENTIFIER_NODE);
            if(identifier != NULL && strcasecmp(identifier->value.string, attr) == 0){
                return getChildNode(attrList->children[i], VALUE_NODE);
            }
        }
    } else{
        logInfo( "WARNING: getFigureAttributeValueNode: Figure attribute list expected. Returning null.\n");
        return NULL;
    }
}

Figure generateFigure(Node * node) {
    char * name = node->value.string;
    Node * type = getChildNode(getFigureAttributeValueNode(node, "type"), STRING_CONSTANT_NODE);
    logDebug("Generating figure: %s (Type: %s)\n", name, type->value.string);
    return NULL;
}

int parseFigureNode(Node * node, U3D_Context * context){
    generateFigure(node);
    return 0;
}