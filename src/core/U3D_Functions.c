#include "U3D_Functions.h"
#include <stdlib.h>
#include <stdio.h>

#include "../adt/variable.h"
#include "../nodes/figureNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int print(size_t paramCount, ParameterValue * values, ParameterType * types){
    if(types[0] == PARAMETER_STRING)
        parse("print(\"%s\");\n", values[0].string);
    else
        parse("print(%s);\n", values[0].string);
    return 0;
}

int setWindowSize(size_t paramCount, ParameterValue * values, ParameterType * types){
    parse("size(%d,%d, P3D);\n", values[0].integer, values[1].integer);
    return 0;
}

int setColorModeToHSB(size_t paramCount, ParameterValue * values, ParameterType * types){
    parse("_ColorModeHSB = true;\n");
    return 0;
}

int setLights(size_t paramCount, ParameterValue * values, ParameterType * types){
    if(types[0] == PARAMETER_INT)
        parse("_LightsOn = %s;\n", values[0].integer == 0? "false" : "true");
    else
        parse("_LightsOn = true;\n");
    return 0;
}

int setBackgroundColor(size_t paramCount, ParameterValue * values, ParameterType * types) {
    if(paramCount == 2) {
        parse("_backgroundColor.x = %d;\n", values[0].vectorInt->x);
        parse("_backgroundColor.y = %d;\n", values[0].vectorInt->y);
        parse("_backgroundColor.z = %d;\n\n", values[0].vectorInt->z);
    } else{
        if(types[0] == PARAMETER_INT)
            parse("_backgroundColor.x = %d;\n", values[0].integer);
        else
            parse("_backgroundColor.x = %s;\n", values[0].string);
        if(types[1] == PARAMETER_INT)
            parse("_backgroundColor.y = %d;\n", values[1].integer);
        else
            parse("_backgroundColor.x = %s;\n", values[1].string);
        if(types[2] == PARAMETER_INT)
            parse("_backgroundColor.z = %d;\n\n", values[2].integer);
        else
            parse("_backgroundColor.x = %s;\n", values[1].string);
    }
    return 0;
}


void initU3DFunctions(U3D_Context * context){

    Function func = newFunction("print");
    addFunctionToTable(func, context);
    addFunctionOverload(func, print, 1, PARAMETER_STRING);

    func = newFunction("drawFigure");
    addFunctionToTable(func, context);
    addFunctionOverload(func, parseDrawFigure, 1, PARAMETER_FIGURE);

    func = newFunction("translateFigure");
    addFunctionToTable(func, context);
    addFunctionOverload(func, parseTranslateFigure, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3);
    addFunctionOverload(func, parseTranslateFigure, 4, PARAMETER_FIGURE, PARAMETER_FLOAT, PARAMETER_FLOAT, PARAMETER_FLOAT);

    func = newFunction("rotateFigure");
    addFunctionToTable(func, context);
    addFunctionOverload(func, parseRotateFigure, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3);
    addFunctionOverload(func, parseRotateFigure, 4, PARAMETER_FIGURE, PARAMETER_FLOAT, PARAMETER_FLOAT, PARAMETER_FLOAT);

    func = newFunction("scaleFigure");
    addFunctionToTable(func, context);
    addFunctionOverload(func, parseScaleFigure, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3);
    addFunctionOverload(func, parseScaleFigure, 4, PARAMETER_FIGURE, PARAMETER_FLOAT, PARAMETER_FLOAT, PARAMETER_FLOAT);

    func = newFunction("addColorFigure");
    addFunctionToTable(func, context);
    addFunctionOverload(func,parseAddColorFigure, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3INT);
    addFunctionOverload(func,parseAddColorFigure, 4, PARAMETER_FIGURE, PARAMETER_INT, PARAMETER_INT, PARAMETER_INT);

    func = newFunction("setFigurePosition");
    addFunctionToTable(func, context);
    addFunctionOverload(func, parseSetFigurePosition, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3);
    addFunctionOverload(func, parseSetFigurePosition, 4, PARAMETER_FIGURE, PARAMETER_FLOAT, PARAMETER_FLOAT, PARAMETER_FLOAT);

    func = newFunction("setFigureRotation");
    addFunctionToTable(func, context);
    addFunctionOverload(func, parseSetFigureRotation, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3);
    addFunctionOverload(func, parseSetFigureRotation, 4, PARAMETER_FIGURE, PARAMETER_FLOAT, PARAMETER_FLOAT, PARAMETER_FLOAT);

    func = newFunction("setFigureScale");
    addFunctionToTable(func, context);
    addFunctionOverload(func, parseSetFigureScale, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3);
    addFunctionOverload(func, parseSetFigureScale, 4, PARAMETER_FIGURE, PARAMETER_FLOAT, PARAMETER_FLOAT, PARAMETER_FLOAT);

    func = newFunction("setFigureColor");
    addFunctionToTable(func, context);
    addFunctionOverload(func,parseSetFigureColor, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3INT);
    addFunctionOverload(func,parseSetFigureColor, 4, PARAMETER_FIGURE, PARAMETER_INT, PARAMETER_INT, PARAMETER_INT);

    func = newFunction("setWindowSize");
    addFunctionToTable(func, context);
    addFunctionOverload(func, setWindowSize, 2, PARAMETER_INT, PARAMETER_INT);

    func = newFunction("setBackground");
    addFunctionToTable(func, context);
    addFunctionOverload(func, setBackgroundColor, 1, PARAMETER_VECTOR3INT);
    addFunctionOverload(func, setBackgroundColor, 3, PARAMETER_INT, PARAMETER_INT, PARAMETER_INT);

    func = newFunction("setColorModeToHSB");
    addFunctionToTable(func, context);
    addFunctionOverload(func, setColorModeToHSB, 0);

    func = newFunction("lights");
    addFunctionToTable(func, context);
    addFunctionOverload(func, setLights, 1, PARAMETER_INT);
}

int nodeToParameterValue(const char * funcName, Node * node, ParameterType expectedType, ParameterValue * value, ParameterType * type, size_t i, U3D_Context * context, bool shouldLog){
    Node * child = node -> children[0];
    Variable var;
    switch (expectedType) {
        case PARAMETER_STRING:
            castNode(child, STRING_CONSTANT_NODE);
            if (child->type == STRING_CONSTANT_NODE) {
                value->string = child->value.string;
                *type = PARAMETER_STRING;
            }
            else if (child->type == IDENTIFIER_NODE){
                var = getVariable(child->value.string, context->first);
                if(var != NULL){
                    if(var->type == VARIABLE_STRING){
                        value->string = child->value.string;
                        *type = PARAMETER_VARIABLE;
                    } else{
                        if(shouldLog)
                            logError(SYNTAX_ERROR, "Expected variable of type String in function: %s; parameter: %d\n", funcName, i);
                        return -1;
                    }
                }
                else{
                    if(shouldLog)
                        logError(SYNTAX_ERROR, "Unknown variable %s in function: %s; parameter %d\n", child, funcName, i);
                    return -1;
                }
            }
            else {
                if(shouldLog)
                    logError(SYNTAX_ERROR, "Expected parameter of type String in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;


        case PARAMETER_INT:
            castNode(child, INTEGER_CONSTANT_NODE);
            if (child->type == INTEGER_CONSTANT_NODE) {
                value->integer = child->value.integer;
                *type = PARAMETER_INT;
            }
            else if (child->type == IDENTIFIER_NODE) {
                var = getVariable(child->value.string, context->first);
                if(var != NULL){
                    if(var->type == VARIABLE_FLOAT || var->type == VARIABLE_INTEGER) {
                        value->string = child->value.string;
                        *type = PARAMETER_VARIABLE;
                    } else {
                        if (shouldLog)
                            logError(SYNTAX_ERROR, "Expected variable of type Integer in function: %s; parameter: %d\n",
                                     funcName, i);
                        return -1;
                    }
                }
            }
            else {
                if(shouldLog)
                    logError(SYNTAX_ERROR, "Expected parameter of type Integer in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;


        case PARAMETER_FLOAT:
            castNode(child, FLOAT_CONSTANT_NODE);
            if (child->type == FLOAT_CONSTANT_NODE) {
                value->floating = child->value.decimal;
                *type = PARAMETER_FLOAT;
            }
            else if (child->type == IDENTIFIER_NODE) {
                var = getVariable(child->value.string, context->first);
                if(var != NULL){
                    if(var->type == VARIABLE_FLOAT || var->type == VARIABLE_INTEGER) {
                        value->string = child->value.string;
                        *type = PARAMETER_VARIABLE;
                    } else {
                        if (shouldLog)
                            logError(SYNTAX_ERROR, "Expected variable of type Float in function: %s; parameter: %d\n",
                                     funcName, i);
                        return -1;
                    }
                }
            }
            else {
                if(shouldLog)
                    logError(SYNTAX_ERROR, "Expected parameter of type Float in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;


        case PARAMETER_VECTOR3:
            castNode(child, VECTOR3_NODE);
            if (child->type == VECTOR3_NODE) {
                value->vector = child->value.vector;
                *type = PARAMETER_VECTOR3;
            }
            else {
                if(shouldLog)
                    logError(SYNTAX_ERROR, "Expected parameter of type Vector3 in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;


        case PARAMETER_VECTOR3INT:
            castNode(child, VECTOR3INT_NODE);
            if (child->type == VECTOR3INT_NODE) {
                value->vectorInt = child->value.vectorInt;
                *type = PARAMETER_VECTOR3INT;
            }
            else {
                if(shouldLog)
                    logError(SYNTAX_ERROR, "Expected parameter of type Vector3Int in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;


        case PARAMETER_BOOLEAN:
            castNode(child, BOOLEAN_CONSTANT_NODE);
            if (child->type == BOOLEAN_CONSTANT_NODE) {
                value->boolean = child->value.boolean;
                *type = PARAMETER_BOOLEAN;
            }
            else if (child->type == IDENTIFIER_NODE) {
                var = getVariable(child->value.string, context->first);
                if(var != NULL){
                    if(var->type == VARIABLE_BOOLEAN) {
                        value->string = child->value.string;
                        *type = PARAMETER_VARIABLE;
                    } else {
                        if (shouldLog)
                            logError(SYNTAX_ERROR, "Expected variable of type Boolean in function: %s; parameter: %d\n",
                                     funcName, i);
                        return -1;
                    }
                }
            }
            else {
                if(shouldLog)
                    logError(SYNTAX_ERROR, "Expected parameter of type Boolean in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;


        case PARAMETER_FIGURE:
            castNode(child, IDENTIFIER_NODE);
            if (child->type == IDENTIFIER_NODE) {
                value->figure = getFigureFromTable(child->value.string, context);
                *type = PARAMETER_FIGURE;
                if (value->figure == NULL){
                    if(shouldLog)
                        logError(SYNTAX_ERROR, "Unknown figure %s in function: %s; parameter: %d\n",child->value.string,  funcName, i);
                    return -1;
                }
            }
            else {
                if(shouldLog)
                    logError(SYNTAX_ERROR, "Expected parameter of type Figure Identifier in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;
        default:
            logInfo("WARNING: nodeToParameterValue(): Impossible state reached");
            return -1;
    }
    return 0;
}

int callU3DFunction(Node * node, U3D_Context * context){
    Node * funcId = getChildNode(node, FUNCTION_IDENTIFIER_NODE);
    Function function = getFunctionFromTable(funcId->value.string, context);
    if(function == NULL){
        logError(SYNTAX_ERROR, "Unknown call to function %s\n", funcId->value.string);
        return -1;
    }

    FunctionOverload * overloads = getFunctionOverloads(function);
    size_t overloadCount = getFunctionOverloadCount(function);
    size_t min = getParameterCount(overloads[0]);

    for(size_t i = 0; i < overloadCount; i++){

        size_t expectedParametersCount = getParameterCount(overloads[i]);
        Node * funcParams = getChildNode(node, PARAMETERS_LIST);
        size_t paramCount = funcParams == NULL? 0 : funcParams->childrenCount;
        if(paramCount != expectedParametersCount){
            min = paramCount > min || expectedParametersCount < min ? expectedParametersCount : min;
            if(i == overloadCount - 1)
                logError(SYNTAX_ERROR, "Function %s: expected %d parameters, received %d\n", getFunctionName(function), min, paramCount);
            continue;
        }

        ParameterType * expectedParameters = getExpectedParameters(overloads[i]);
        ParameterValue parameterValueArray[paramCount];
        ParameterType parameterTypeArray[paramCount];

        bool cont = false;
        for(size_t j = 0; j < paramCount; j++){
            if(nodeToParameterValue(getFunctionName(function), funcParams->children[j], expectedParameters[j], &parameterValueArray[j], &parameterTypeArray[j],
                                    j, context, i == overloadCount - 1) < 0){
                cont = true;
            }
        }

        if(cont)
            continue;

        return executeFunction(overloads[i], paramCount, parameterValueArray, parameterTypeArray);
    }

    return -1;
}