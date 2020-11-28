#include "u3d_context.h"
#include "utils/logger.h"
#include <string.h>
#include <malloc.h>

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

int newVariable(char * identifier, enum VariableType type, union VariableValue value, U3D_Context * context) {

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
    
    if(insertNewVariable(&context->first, identifier, type, value) == NULL) {
        logError(FATAL_ERROR, "Couldn't define variable \"%s\"\n", identifier);
        return -1;
    }

    return 0;
}