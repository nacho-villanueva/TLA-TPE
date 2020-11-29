#include <malloc.h>
#include <strings.h>
#include <string.h>
#include "figureNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

FigureType getFigureType(char * type){
    if(strcasecmp(type, "box") == 0)
        return FIGURE_BOX;
    if(strcasecmp(type, "sphere") == 0)
        return FIGURE_SPHERE;
    if(strcasecmp(type, "pyramid") == 0)
        return FIGURE_PYRAMID;
    if(strcasecmp(type, "composite") == 0)
        return FIGURE_COMPOSITE;
    if(strcasecmp(type, "custom") == 0)
        return FIGURE_CUSTOM;
    return FIGURE_INVALID;
}

FigureAttributeType getFigureAttributeType(char * type){
    if(strcasecmp(type, "scale") == 0)
        return ATTR_SCALE;
    if(strcasecmp(type, "position") == 0)
        return ATTR_POSITION;
    if(strcasecmp(type, "rotation") == 0)
        return ATTR_ROTATION;
    if(strcasecmp(type, "color") == 0)
        return ATTR_COLOR;
    if(strcasecmp(type, "child") == 0)
        return ATTR_CHILD;
    if(strcasecmp(type, "path") == 0)
        return ATTR_PATH;
    return ATTR_INVALID;
}

Node * getFigureAttributeValueNode(Node * node, char * attr){
    Node * attrList = getChildNode(node, FIGURE_ATTRIBUTE_LIST_NODE);
    if(attrList != NULL){
        for(int i = 0; i < attrList->childrenCount; i++){
            Node * identifier = getChildNode(attrList->children[i], IDENTIFIER_NODE);
            if(identifier != NULL && strcasecmp(identifier->value.string, attr) == 0){
                return getChildNode(attrList->children[i], VALUE_NODE);
            }
        }
    } else{
        logInfo( "WARNING: getFigureAttributeValueNode: Figure attribute list expected. Returning null.\n");
        return NULL;
    }

    logInfo("WARNING: getFigureAttributeValueNode(): Impossible scenario\n");
    return NULL;
}

void setFigureAttributeFromValueNode(Figure figure, FigureAttributeType type, Node * valueNode, U3D_Context * context){ // Ya ni intento por buen nombres...
    FigureAttributeValue attrValue;
    Node * vn = NULL;
    switch (type) {
        case ATTR_SCALE:
        case ATTR_POSITION:
        case ATTR_ROTATION:
            vn = valueNode->children[0];
            castNode(vn, VECTOR3_NODE);
            if(vn == NULL || vn->type != VECTOR3_NODE) {
                logError(SYNTAX_ERROR, "Wrong attribute type in Figure: %s. Expected value type: Vector3.\n",
                         getFigureName(figure));    //TODO: MESSAGE ERROR CAN BE IMPROVED
                return;
            }
            attrValue.vector = vn->value.vector;
            if(attrValue.vector != NULL)
                setFigureAttribute(figure, type, attrValue);
            break;
        case ATTR_COLOR:
            vn = valueNode->children[0];
            castNode(vn, VECTOR3INT_NODE);
            if(vn == NULL || vn->type != VECTOR3INT_NODE) {
                logError(SYNTAX_ERROR, "Wrong attribute type in Figure: %s. Expected value type: Vector3Int.\n",
                         getFigureName(figure));    //TODO: MESSAGE ERROR CAN BE IMPROVED
                return;
            }
            attrValue.vectorInt = vn->value.vectorInt;
            if(attrValue.vector != NULL)
                setFigureAttribute(figure, type, attrValue);
            break;
        case ATTR_CHILD:
            vn = getChildNode(valueNode, IDENTIFIER_NODE);
            if(vn == NULL) {
                logError(SYNTAX_ERROR, "Wrong attribute type in Figure: %s. Expected value type: Identifier.\n",
                         getFigureName(figure));    //TODO: MESSAGE ERROR CAN BE IMPROVED
                return;
            }
            attrValue.figure = getFigureFromTable(vn -> value.string, context);
            if(attrValue.figure != NULL)
                setFigureAttribute(figure, type, attrValue);
            else
                logWarning("Unknown figure name %s in figure %s. Skipping.\n", vn -> value.string, getFigureName(figure));
            break;
        case ATTR_PATH:
            vn = getChildNode(valueNode, STRING_CONSTANT_NODE);
            if(vn == NULL) {
                logError(SYNTAX_ERROR, "Wrong attribute type in Figure: %s. Expected value type: String.\n",
                         getFigureName(figure));    //TODO: MESSAGE ERROR CAN BE IMPROVED
                return;
            }
            attrValue.path = vn -> value.string;
            if(attrValue.path != NULL)
                setFigureAttribute(figure, type, attrValue);
            break;
        default:
        case ATTR_INVALID:
            logInfo("WARNING: Setting invalid figure attribute type\n");
            return;
    }
}

void setAllAttributes(Node * node, Figure figure, U3D_Context * context){
    Node * attrList = getChildNode(node, FIGURE_ATTRIBUTE_LIST_NODE);
    if(attrList != NULL){
        for(int i = 0; i < attrList->childrenCount; i++){
            Node * identifier = getChildNode(attrList->children[i], IDENTIFIER_NODE);
            if(identifier != NULL){
                FigureAttributeType type = getFigureAttributeType(identifier->value.string);
                if(type != ATTR_INVALID){
                    Node * value = getChildNode(attrList->children[i], VALUE_NODE);
                    setFigureAttributeFromValueNode(figure, type, value, context);
                }
            }
        }
    } else{
        logInfo( "WARNING: setAllAttributes: Figure attribute list expected. Returning null.\n");
    }
}

Figure generateFigure(Node * node, U3D_Context * context) {
    char * name = node->value.string;
    Node * type = getFigureAttributeValueNode(node, "type");
    if(type == NULL){
        logError(SYNTAX_ERROR, "No type defined for Figure %s\n", name);
        return NULL;
    }
    Node * typeValue = getChildNode(type, IDENTIFIER_NODE);
    if(typeValue == NULL){
        logError(SYNTAX_ERROR, "Figure %s Type invalid type. Expected an identifier.\n", name);
        return NULL;
    }
    FigureType figureType = getFigureType(typeValue->value.string);
    if(figureType == FIGURE_INVALID){
        logError(SYNTAX_ERROR, "Invalid figure type %s\n", typeValue);
        return NULL;
    }

    logDebug("Generating figure: %s (Type: %s)\n", name, typeValue->value.string);
    Figure figure = newFigure(name, figureType);
    setAllAttributes(node, figure, context);

    printFigure(figure); //TODO: REMOVE

    return figure;
}

int parseFigureNode(Node * node, U3D_Context * context){
    Figure figure = generateFigure(node, context);
    if(figure == NULL)
        return -1;

    if(addFigureToTable(figure, context) < 0){
        freeFigure(figure);
        return -1;
    }

    parse("Figure %s = new Figure();\n", getFigureName(figure));
    return 0;
}

void parseFiguresInit(U3D_Context * context){
    for(size_t i = 0; i < context->figuresCount; i++){
        parseFigureAttributes(context->figuresTable[i]);
    }
}

int parseDrawFigure(size_t paramCount, ParameterValue * values, ParameterType * types) {
    if(paramCount != 1)
        return -1;
    if(types[0] != PARAMETER_FIGURE)
        return -1;

    drawFigure(values[0].figure);
    return 0;
}

int parseModifyFigurePosition(size_t paramCount, ParameterValue * values, ParameterType * types, bool shouldSet){
    char * set = shouldSet ? "" : "+";
    if(paramCount == 2) {
        char * figureName = getFigureName(values[0].figure);
        parse("%s.fposition.x %s= %f;\n", figureName, set, values[1].vector->x);
        parse("%s.fposition.y %s= %f;\n", figureName, set, values[1].vector->y);
        parse("%s.fposition.z %s= %f;\n\n", figureName, set, values[1].vector->z);
        return 0;
    } else if(paramCount == 4){
        char * figureName = getFigureName(values[0].figure);

        parse("%s.fposition.x %s= ", figureName, set);
        if(types[1] == PARAMETER_VARIABLE)
            parse("%s;\n", values[1].string);
        else
            parse("%f;\n", values[1].floating);

        parse("%s.fposition.y %s= ", figureName, set);
        if(types[2] == PARAMETER_VARIABLE)
            parse("%s;\n", values[2].string);
        else
            parse("%f;\n", values[2].floating);

        parse("%s.fposition.z %s= ", figureName,set);
        if(types[3] == PARAMETER_VARIABLE)
            parse("%s;\n\n", values[3].string);
        else
            parse("%f;\n\n", values[3].floating);

        return 0;
    }

    return -1;
}

int parseModifyFigureRotation(size_t paramCount, ParameterValue * values, ParameterType * types, bool shouldSet){
    char * set = shouldSet ? "" : "+";
    if(paramCount == 2) {
        char * figureName = getFigureName(values[0].figure);
        parse("%s.frotation.x %s= %f;\n", figureName, set, values[1].vector->x);
        parse("%s.frotation.y %s= %f;\n", figureName, set, values[1].vector->y);
        parse("%s.frotation.z %s= %f;\n\n", figureName, set, values[1].vector->z);
        return 0;
    } else if(paramCount == 4){
        char * figureName = getFigureName(values[0].figure);

        parse("%s.frotation.x %s= ", figureName, set);
        if(types[1] == PARAMETER_VARIABLE)
            parse("%s;\n", values[1].string);
        else
            parse("%f;\n", values[1].floating);

        parse("%s.frotation.y %s= ", figureName, set);
        if(types[2] == PARAMETER_VARIABLE)
            parse("%s;\n", values[2].string);
        else
            parse("%f;\n", values[2].floating);

        parse("%s.frotation.z %s= ", figureName, set);
        if(types[3] == PARAMETER_VARIABLE)
            parse("%s;\n\n", values[3].string);
        else
            parse("%f;\n\n", values[3].floating);

        return 0;
    }

    return -1;
}

int parseModifyFigureScale(size_t paramCount, ParameterValue * values, ParameterType * types, bool shouldSet){
    char * set = shouldSet ? "" : "+";
    if(paramCount == 2) {
        char * figureName = getFigureName(values[0].figure);
        parse("%s.fscale.x %s= %f;\n", figureName, set, values[1].vector->x);
        parse("%s.fscale.y %s= %f;\n", figureName, set, values[1].vector->y);
        parse("%s.fscale.z %s= %f;\n\n", figureName, set, values[1].vector->z);
        return 0;
    } else if(paramCount == 4){
        char * figureName = getFigureName(values[0].figure);

        parse("%s.fscale.x %s= ", figureName, set);
        if(types[1] == PARAMETER_VARIABLE)
            parse("%s;\n", values[1].string);
        else
            parse("%f;\n", values[1].floating);

        parse("%s.fscale.y %s= ", figureName, set);
        if(types[2] == PARAMETER_VARIABLE)
            parse("%s;\n", values[2].string);
        else
            parse("%f;\n", values[2].floating);

        parse("%s.fscale.z %s= ", figureName, set);
        if(types[3] == PARAMETER_VARIABLE)
            parse("%s;\n\n", values[3].string);
        else
            parse("%f;\n\n", values[3].floating);

        return 0;
    }

    return -1;
}

int parseModifyFigureColor(size_t paramCount, ParameterValue * values, ParameterType * types, bool shouldSet){
    char * set = shouldSet ? "" : "+";
    if(paramCount == 2) {
        char * figureName = getFigureName(values[0].figure);
        parse("%s.fcolor.x %s= %d;\n", figureName, set, values[1].vectorInt->x);
        parse("%s.fcolor.y %s= %d;\n", figureName, set, values[1].vectorInt->y);
        parse("%s.fcolor.z %s= %d;\n\n", figureName, set, values[1].vectorInt->z);
        return 0;
    } else if(paramCount == 4){
        char * figureName = getFigureName(values[0].figure);

        parse("%s.fcolor.x %s= ", figureName, set);
        if(types[1] == PARAMETER_VARIABLE)
            parse("%s;\n", values[1].string);
        else
            parse("%d;\n", values[1].floating);

        parse("%s.fcolor.y %s= ", figureName, set);
        if(types[2] == PARAMETER_VARIABLE)
            parse("%s;\n", values[2].string);
        else
            parse("%d;\n", values[2].floating);

        parse("%s.fcolor.z %s= ", figureName, set);
        if(types[3] == PARAMETER_VARIABLE)
            parse("%s;\n\n", values[3].string);
        else
            parse("%d;\n\n", values[3].floating);

        return 0;
    }

    return -1;
}



int parseTranslateFigure(size_t paramCount, ParameterValue * values, ParameterType * types) {
    return parseModifyFigurePosition(paramCount, values, types, false);
}

int parseRotateFigure(size_t paramCount, ParameterValue * values, ParameterType * types) {
    return parseModifyFigureRotation(paramCount, values, types, false);
}

int parseScaleFigure(size_t paramCount, ParameterValue * values, ParameterType * types) {
    return parseModifyFigureScale(paramCount, values, types, false);
}

int parseAddColorFigure(size_t paramCount, ParameterValue * values, ParameterType * types) {
    return parseModifyFigureColor(paramCount, values, types, false);
}

int parseSetFigurePosition(size_t paramCount, ParameterValue * values, ParameterType * types){
    return parseModifyFigurePosition(paramCount, values, types, true);
}

int parseSetFigureRotation(size_t paramCount, ParameterValue * values, ParameterType * types){
    return parseModifyFigureRotation(paramCount, values, types, true);
}

int parseSetFigureScale(size_t paramCount, ParameterValue * values, ParameterType * types){
    return parseModifyFigureScale(paramCount, values, types, true);
}

int parseSetFigureColor(size_t paramCount, ParameterValue * values, ParameterType * types){
    return parseModifyFigureColor(paramCount, values, types, true);
}


