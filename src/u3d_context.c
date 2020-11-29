#include "u3d_context.h"

#include "adt/figure.h"
#include "adt/function.h"

#include <string.h>
#include <malloc.h>
#include "utils/logger.h"

bool isVariableDefined(char * name, U3D_Context * context) {
    for (size_t i = 0; i < context->figuresCount; ++i)
        if(strcmp(getFigureName(context->figuresTable[i]), name) == 0)
            return true;

    // We check on the linked list of variables as well
    int ret = checkIfIdentifierIsUsed(name, context->first);
    if(ret == 1)
        return true;
        
    return false;
}

bool isFunctionDefined(char * name, U3D_Context * context) {
    for (size_t i = 0; i < context->functionCount; ++i) {
        if(strcmp(getFunctionName(context->functionTable[i]), name) == 0)
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

    logError(ERROR, "Variable \"%s\" redefinition.\n", getFigureName(figure));
    return -1;
}

int addFunctionToTable(Function function, U3D_Context *context) {
    if(!isFunctionDefined(getFunctionName(function), context)) {
        context->functionCount++;
        context->functionTable = reallocarray(context->functionTable, context->functionCount, sizeof(Function));
        context->functionTable[context->functionCount - 1] = function;
        return 0;
    }

    logError(ERROR, "Function %s redefinition.\n", getFunctionName(function));
    return -1;
}

Function getFunctionFromTable(char * name, U3D_Context *context) {
    for(size_t i = 0; i < context->functionCount; i++){
        if(strcmp(getFunctionName(context->functionTable[i]), name) == 0){
            return context->functionTable[i];
        }
    }
    return NULL;
}

