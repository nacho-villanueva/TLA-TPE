#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <stdbool.h>

typedef struct Variable * Variable;

enum VariableType{
    VARIABLE_STRING = 0,
    VARIABLE_INTEGER,
    VARIABLE_FLOAT,
    VARIABLE_BOOLEAN,
};

union VariableValue {
    char * string;
    int integer;
    float decimal;
    bool boolean;
    void * notFound;
};

void freeVariable(char * identifier);

struct Variable * insertNewVariable(struct Variable ** first_variable, char * identifier, enum VariableType type, union VariableValue value, bool isConstant);

int checkIfIdentifierIsUsed(char * identifier, Variable first);

union VariableValue getVariableValue(char * identifier, Variable first);

int isConstant(char * identifier, struct Variable * first_variable);

int setVariableValue(char * identifier, enum VariableType type, union VariableValue value, Variable first);

enum VariableType getVariableType(char * identifier, struct Variable * first);

#endif 
