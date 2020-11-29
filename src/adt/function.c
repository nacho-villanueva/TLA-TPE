#include "function.h"
#include <stdarg.h>

struct functionCDT{
    char * name;
    int (*function) (ParameterValue parameterValuesArray[]);
    size_t parameterCount;
    ParameterType parameters[];
};

Function newFunction(char *name, void *function_ptr, size_t paramCount, ...) {
    Function function = malloc(sizeof(name) + sizeof(function_ptr) + sizeof(paramCount) + sizeof(ParameterType) * paramCount);

    function->name = name;
    function->function = function_ptr;
    function->parameterCount = paramCount;

    va_list args;
    va_start(args, paramCount);
    for (size_t i = 0; i < paramCount; i++) {
        function->parameters[i] = va_arg(args, ParameterType);
    }
    va_end(args);

    return function;
}

char * getFunctionName(Function function){
    return function->name;
}

void freeFunction(Function function){
    free(function);
}

size_t getParameterCount(Function function) {
    return function->parameterCount;
}

ParameterType * getExpectedParameters(Function function) {
    return function->parameters;
}

int executeFunction(Function function, ParameterValue * parameterValueArray) {
    return function->function(parameterValueArray);
}