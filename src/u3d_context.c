#include "u3d_context.h"
#include "utils/logger.h"
#include <string.h>
#include <malloc.h>

bool isVariableDefined(char * name, U3D_Context * context) {
    for (size_t i = 0; i < context->figuresCount; ++i) {
        if(strcmp(getFigureName(context->figuresTable[i]), name) == 0)
            return true;
    }

    return false;
}

Figure getFigureFromTable(char *name, U3D_Context *context) {
    for(size_t i = 0; i < context->figuresCount; i++){
        if(strcmp(getFigureName(context->figuresTable[i]), name) == 0){
            return context->figuresTable[i];
        }
    }
    return NULL;
}

int addFigureToTable(Figure figure, U3D_Context *context) {
    if(!isVariableDefined(getFigureName(figure), context)) {
        context->figuresCount++;
        context->figuresTable = reallocarray(context->figuresTable, context->figuresCount, sizeof(Figure));
        context->figuresTable[context->figuresCount - 1] = figure;
        return 0;
    }

    logError(ERROR, "Variable %s redefinition.\n", getFigureName(figure));
    return -1;
}

