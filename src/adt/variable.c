#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/logger.h"
#include "variable.h"

struct Variable * insertNewVariable(struct Variable ** first_variable, char * identifier, enum VariableType type, bool isConstant, bool isInitialized) {
    if((*first_variable) == NULL) {
        (*first_variable) = calloc(1, sizeof(struct Variable));
        if((*first_variable) == NULL) {
            logError(ERROR, "variable.c: Cannot allocate memory");
            return NULL;
        }
        (*first_variable)->type = type;
        (*first_variable)->isConstant = isConstant;
        (*first_variable)->identifier = malloc(strlen(identifier) + 1);
        (*first_variable)->isInitialized = isInitialized;
        if((*first_variable)->identifier == NULL) {
            logError(ERROR, "variable.c: Cannot allocate memory");
            return NULL;
        }
        strcpy((*first_variable)->identifier, identifier);
        (*first_variable)->nextVariable = NULL;
        return (*first_variable);
    }

    if(first_variable != NULL && (*first_variable)->nextVariable == NULL) {
        (*first_variable)->nextVariable = insertNewVariable(&(*first_variable)->nextVariable, identifier, type, isConstant, isInitialized);
        return (*first_variable)->nextVariable;
    }

    return insertNewVariable(&(*first_variable)->nextVariable, identifier, type, isConstant, isInitialized);
}

Variable getVariable(char * identifier, Variable first) {
    if(first == NULL)
        return NULL;
    
    if(strcmp(first->identifier, identifier) == 0)
        return first;
    
    return getVariable(identifier, first->nextVariable);
}
