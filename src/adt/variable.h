#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <stdbool.h>

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

int newVariable(char * identifier, enum VariableType type, union VariableValue value);

void freeVariable(char * identifier);

int checkIfIdentifierIsUsed(char * identifier);

union VariableValue getVariableValue(char * identifier);

int setVariableValue(char * identifier, enum VariableType type, union VariableValue value);

#endif 
