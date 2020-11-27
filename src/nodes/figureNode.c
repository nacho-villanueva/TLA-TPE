#include <stdlib.h>
#include <malloc.h>
#include "figureNode.h"
#include "../utils/logger.h"

void addFigureToTable(Figure figure, U3D_Context * context) {
    context->figuresCount++;
    context->figuresTable = reallocarray(context->figuresTable, context->figuresCount, sizeof(Figure));
    context->figuresTable[context->figuresCount-1] = figure;
}

Figure generateFigure(Node * node) {
    char * name = node->value.string;
    logDebug("Generating figure: %s\n", name);
    return NULL;
}

int parseFigureNode(Node * node, U3D_Context * context){
    generateFigure(node);
    return 0;
}