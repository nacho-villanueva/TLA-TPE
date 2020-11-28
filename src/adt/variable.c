#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/logger.h"
#include "../utils/parser.h"
#include "variable.h"
#include "../u3d_context.h"

// Prototypes
static union VariableValue getVariableValueRec(struct Variable * first, char * identifier);
static int setVariableValueRec(struct Variable * first, char * identifier, enum VariableType type, union VariableValue value);
static int checkIfIdentifierIsUsedRec(struct Variable * first, char * identifier);
static enum VariableType getVariableTypeRec(struct Variable * first_variable, char * identifier);


struct Variable{
    enum VariableType type;
    union VariableValue value;
    char * identifier;
    struct Variable * nextVariable;
};

//struct Variable * first = NULL;

int checkIfIdentifierIsUsed(char * identifier, Variable first) {
    return checkIfIdentifierIsUsedRec(first, identifier);
}

static int checkIfIdentifierIsUsedRec(struct Variable * first_variable, char * identifier) {
    if(first_variable == NULL)
        return 0;

    if(strcmp(first_variable->identifier, identifier) == 0)
        return 1;

    return checkIfIdentifierIsUsedRec(first_variable->nextVariable, identifier);
}

struct Variable * insertNewVariable(struct Variable ** first_variable, char * identifier, enum VariableType type, union VariableValue value) {

    if((*first_variable) == NULL) {
        (*first_variable) = calloc(1, sizeof(struct Variable));
        if((*first_variable) == NULL) {
            logError(ERROR, "variable.c: Cannot allocate memory");
            return NULL;
        }
        (*first_variable)->type = type;
        (*first_variable)->value = value;
        (*first_variable)->identifier = malloc(strlen(identifier) + 1);
        if((*first_variable)->identifier == NULL) {
            logError(ERROR, "variable.c: Cannot allocate memory");
            return NULL;
        }
        strcpy((*first_variable)->identifier, identifier);
        (*first_variable)->nextVariable = NULL;
        return (*first_variable);
    }

    if(first_variable != NULL && (*first_variable)->nextVariable == NULL) {
        (*first_variable)->nextVariable = insertNewVariable(&(*first_variable)->nextVariable, identifier, type, value);
        return (*first_variable)->nextVariable;
    }

    return insertNewVariable(&(*first_variable)->nextVariable, identifier, type, value);
}

union VariableValue getVariableValue(char * identifier, struct Variable * first) {
    return getVariableValueRec(first, identifier);
}

static union VariableValue getVariableValueRec(struct Variable * first_variable, char * identifier) {
    if (first_variable == NULL) {
        union VariableValue error;
        error.notFound = NULL;
        return error;
    }
    
    if(strcmp(first_variable->identifier, identifier) == 0)
        return first_variable->value;
    
    return getVariableValueRec(first_variable->nextVariable, identifier);
}

enum VariableType getVariableType(char * identifier, struct Variable * first) {
    return getVariableTypeRec(first, identifier);
}

static enum VariableType getVariableTypeRec(struct Variable * first_variable, char * identifier) {
    if (first_variable == NULL)
        return -1;
    
    if(strcmp(first_variable->identifier, identifier) == 0)
        return first_variable->type;
    
    return getVariableTypeRec(first_variable->nextVariable, identifier);
}

int setVariableValue(char * identifier, enum VariableType type, union VariableValue value, struct Variable * first) {
    return setVariableValueRec(first, identifier, type, value);
}

static int setVariableValueRec(struct Variable * first_variable, char * identifier, enum VariableType type, union VariableValue value) {
    if(first_variable == NULL)
        return -1;
        
    if(strcmp(first_variable->identifier, identifier) == 0) {
        if(first_variable->type == type) {
            first_variable->value = value;
            return 0;
        } else {
            return -1;
        }
    }
        
    return setVariableValueRec(first_variable->nextVariable, identifier, type, value);
}