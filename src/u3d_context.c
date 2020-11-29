#include "u3d_context.h"

#include "adt/figure.h"
#include "adt/function.h"
#include "adt/variable.h"

#include <string.h>
#include <malloc.h>
#include "utils/logger.h"

bool isVariableDefined(char * name, U3D_Context * context) {
    for (size_t i = 0; i < context->figuresCount; ++i)
        if(strcmp(getFigureName(context->figuresTable[i]), name) == 0)
            return true;

    // We check on the linked list of variables as well
    if(getVariable(name, context->first) != NULL)
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
        context->figuresTable = realloc(context->figuresTable, context->figuresCount * sizeof(Figure));
        context->figuresTable[context->figuresCount - 1] = figure;
        return 0;
    }

    logError(ERROR, "Variable \"%s\" redefinition.\n", getFigureName(figure));
    return -1;
}

int addFunctionToTable(Function function, U3D_Context *context) {
    if(!isFunctionDefined(getFunctionName(function), context)) {
        context->functionCount++;
        context->functionTable = realloc(context->functionTable, context->functionCount * sizeof(Function));
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

int newVariable(char * identifier, enum VariableType type, U3D_Context * context, bool isConstant, bool isInitialized) {

    // TODO: palabras reservadas
    if(strcmp(identifier, "if") == 0 || strcmp(identifier, "float") == 0 || strcmp(identifier, "string") == 0 ||
       strcmp(identifier, "figure") == 0 || strcmp(identifier, "boolean") == 0 || strcmp(identifier, "function") == 0 ||
       strcmp(identifier, "while") == 0 || strcmp(identifier, "int") == 0) {
        logError(SYNTAX_ERROR, "\"%s\" is a reserved word\n", identifier);
        return -1;
    }

    // Hay que chequear que el nombre no exista ya
    if(isVariableDefined(identifier, context)) {
        logError(SYNTAX_ERROR, "Variable \"%s\" already defined\n", identifier);
        return -1;
    }

    if(insertNewVariable(&context->first, identifier, type, isConstant, isInitialized) == NULL) {
        logError(FATAL_ERROR, "Couldn't define variable \"%s\"\n", identifier);
        return -1;
    }

    return 0;
}