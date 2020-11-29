#include "function.h"
#include <stdarg.h>

struct functionOverloadCDT {
    int (*function) (size_t, ParameterValue[], ParameterType[]);
    size_t parameterCount;
    ParameterType parameters[];
};

struct functionCDT{
    char * name;
    size_t overloadCount;
    FunctionOverload * overloads;
};

Function newFunction(char *name) {
    Function function = calloc(1, sizeof(struct functionCDT));

    function->name = name;
    function->overloadCount = 0;

    return function;
}

void addFunctionOverload(Function function, int (*pFunction)(size_t, ParameterValue[], ParameterType[]), size_t paramCount, ...){
    function->overloadCount++;
    function->overloads = realloc(function->overloads, function->overloadCount * sizeof(FunctionOverload));
    function->overloads[function->overloadCount - 1] = calloc(1, sizeof(pFunction) + sizeof(size_t) + paramCount * sizeof(ParameterType));

    FunctionOverload overload = function->overloads[function->overloadCount - 1];

    overload->parameterCount = paramCount;
    overload->function = pFunction;

    if(paramCount > 0) {
        va_list args;
        va_start(args, paramCount);
        for (size_t i = 0; i < paramCount; i++) {
            overload->parameters[i] = va_arg(args, ParameterType);
        }
        va_end(args);
    }
}

char * getFunctionName(Function function){
    return function->name;
}

void freeFunction(Function function){
    if(function != NULL) {
        if(function->overloadCount > 0 && function->overloads != NULL){
            for(size_t i =0; i < function->overloadCount; i++){
                free(function->overloads[i]);
            }
            free(function->overloads);
        }
        free(function);
    }
}

size_t getParameterCount(FunctionOverload overload) {
    return overload -> parameterCount;
}

ParameterType * getExpectedParameters(FunctionOverload overload) {
    return overload->parameters;
}

int executeFunction(FunctionOverload overload, size_t paramCount, ParameterValue * parameterValueArray, ParameterType * type) {
    return overload->function(paramCount, parameterValueArray, type);
}

size_t getFunctionOverloadCount(Function function) {
    return function->overloadCount;
}


FunctionOverload *getFunctionOverloads(Function function) {
    return function->overloads;
}
