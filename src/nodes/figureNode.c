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

int parseDrawFigure(ParameterValue * values) {
    drawFigure(values[0].figure);
    return 0;
}

int parseTranslateFigure(ParameterValue *values) {
    translateFigure(values[0].figure, values[1].vector);
    return 0;
}

int parseRotateFigure(ParameterValue *values) {
    rotateFigure(values[0].figure, values[1].vector);
    return 0;
}

int parseScaleFigure(ParameterValue *values) {
    scaleFigure(values[0].figure, values[1].vector);
    return 0;
}

int parseAddColorFigure(ParameterValue *values) {
    addColorFigure(values[0].figure, values[1].vectorInt);
    return 0;
}
