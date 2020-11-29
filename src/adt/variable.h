#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <stdbool.h>

static const char * reserved_words[] = {
    "if",
    "float",
    "string",
    "figure",
    "boolean",
    "function",
    "while",
    "int"
};

enum VariableType{
    VARIABLE_STRING = 0,
    VARIABLE_INTEGER,
    VARIABLE_FLOAT,
    VARIABLE_BOOLEAN,
};

struct Variable{
    enum VariableType type;
    char * identifier;
    struct Variable * nextVariable;
    bool isConstant;
    bool isInitialized;
};

typedef struct Variable * Variable;

void freeVariable(char * identifier);

struct Variable * insertNewVariable(struct Variable ** first_variable, char * identifier, enum VariableType type, bool isConstant, bool isInitialized);

Variable getVariable(char * identifier, Variable first);

#endif 
