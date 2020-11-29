#include "U3D_Functions.h"
#include <stdlib.h>
#include <stdio.h>

#include "../nodes/figureNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int print(NodeValue * values){
    parse("System.out.println(%s);\n", values[0].string);
    return 0;
}

void initU3DFunctions(U3D_Context * context){
    addFunctionToTable(newFunction("print", print, 1, PARAMETER_STRING), context);
    addFunctionToTable(newFunction("drawFigure", parseDrawFigure, 1, PARAMETER_FIGURE), context);

    addFunctionToTable(newFunction("translateFigure", parseTranslateFigure, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3), context);
    addFunctionToTable(newFunction("rotateFigure", parseRotateFigure, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3), context);
    addFunctionToTable(newFunction("scaleFigure", parseScaleFigure, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3), context);
    addFunctionToTable(newFunction("addColorFigure", parseAddColorFigure, 2, PARAMETER_FIGURE, PARAMETER_VECTOR3INT), context);
}

int nodeToParameterValue(const char * funcName, Node * node, ParameterType expectedType, ParameterValue * value, size_t i, U3D_Context * context){
    Node * child = node -> children[0];
    switch (expectedType) {
        case PARAMETER_STRING:
            castNode(child, STRING_CONSTANT_NODE);
            if (child->type == STRING_CONSTANT_NODE)
                value->string = child->value.string;
            else {
                logError(SYNTAX_ERROR, "Expected parameter of type String in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;

        case PARAMETER_INT:
            castNode(child, INTEGER_CONSTANT_NODE);
            if (child->type == INTEGER_CONSTANT_NODE)
                value->integer = child->value.integer;
            else {
                logError(SYNTAX_ERROR, "Expected parameter of type Integer in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;

        case PARAMETER_FLOAT:
            castNode(child, FLOAT_CONSTANT_NODE);
            if (child->type == FLOAT_CONSTANT_NODE)
                value->floating = child->value.decimal;
            else {
                logError(SYNTAX_ERROR, "Expected parameter of type Float in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;

        case PARAMETER_VECTOR3:
            castNode(child, VECTOR3_NODE);
            if (child->type == VECTOR3_NODE)
                value->vector = child->value.vector;
            else {
                logError(SYNTAX_ERROR, "Expected parameter of type Vector3 in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;

        case PARAMETER_VECTOR3INT:
            castNode(child, VECTOR3INT_NODE);
            if (child->type == VECTOR3INT_NODE)
                value->vectorInt = child->value.vectorInt;
            else {
                logError(SYNTAX_ERROR, "Expected parameter of type Vector3Int in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;

        case PARAMETER_BOOLEAN:
            castNode(child, BOOLEAN_CONSTANT_NODE);
            if (child->type == BOOLEAN_CONSTANT_NODE)
                value->boolean = child->value.boolean;
            else {
                logError(SYNTAX_ERROR, "Expected parameter of type Boolean in function: %s; parameter: %d\n", funcName, i);
                return -1;
            }
            break;

        case PARAMETER_FIGURE:
            castNode(child, IDENTIFIER_NODE);
            if (child->type == IDENTIFIER_NODE) {
                value->figure = getFigureFromTable(child->value.string, context);
                if (value->figure == NULL){
                    logError(SYNTAX_ERROR, "Unknown figure %s in function: %s; parameter: %d\n",child->value.string,  funcName, i);
                    return -1;
                }
            }
            else {
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

    size_t expectedParametersCount = getParameterCount(function);
    Node * funcParams = getChildNode(node, PARAMETERS_LIST);
    size_t paramCount = funcParams == NULL? 0 : funcParams->childrenCount;
    if(paramCount != expectedParametersCount){
        logError(SYNTAX_ERROR, "Function %s: expected %d parameters, received %d\n", getFunctionName(function), expectedParametersCount, paramCount);
        return -1;
    }
    ParameterType * expectedParameters = getExpectedParameters(function);
    ParameterValue parameterValueArray[paramCount];
    for(size_t i = 0; i < paramCount; i++){
        if(nodeToParameterValue(getFunctionName(function), funcParams->children[i], expectedParameters[i], &parameterValueArray[i], i, context) < 0){
            return -1;
        }
    }

    return executeFunction(function, parameterValueArray);

}