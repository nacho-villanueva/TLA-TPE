#include <malloc.h>
#include <strings.h>
#include <string.h>
#include "../adt/figure.h"
#include "figureNode.h"
#include "../utils/logger.h"


void addFigureToTable(Figure figure, U3D_Context * context) {
    context->figuresCount++;
    context->figuresTable = reallocarray(context->figuresTable, context->figuresCount, sizeof(Figure));
    context->figuresTable[context->figuresCount-1] = figure;
}

Figure getFigureFromTable(char* name, U3D_Context * context){
    for(size_t i = 0; i < context->figuresCount; i++){
        if(strcmp(getFigureName(context->figuresTable[i]), name) == 0){
            return context->figuresTable[i];
        }
    }
    return NULL;
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

void setFigureAttributeFromValueNode(Figure figure, FigureAttributeType type, Node * valueNode, U3D_Context * context){ // Ya ni intento por buen nombres...
    FigureAttributeValue attrValue;
    Node * vn = NULL;
    switch (type) {
        case ATTR_SCALE:
        case ATTR_POSITION:
        case ATTR_ROTATION:
            vn = getChildNode(valueNode, VECTOR3_NODE);
            if(vn == NULL) {
                vn = getChildNode(valueNode, VECTOR3INT_NODE);
                if(vn == NULL) {
                    logError(SYNTAX_ERROR, "Wrong attribute type in Figure: %s. Expected value type: Vector3.\n",
                             getFigureName(figure));    //TODO: MESSAGE ERROR CAN BE IMPROVED
                    return;
                }
                vn->type = VECTOR3_NODE;
                vn->value.vector = vector3IntToVector3(vn->value.vectorInt);
            }
            attrValue.vector = vn->value.vector;
            if(attrValue.vector != NULL)
                setFigureAttribute(figure, type, attrValue);
            break;
        case ATTR_COLOR:
            vn = getChildNode(valueNode, VECTOR3INT_NODE);
            if(vn == NULL) {
                logError(SYNTAX_ERROR, "Wrong attribute type in Figure: %s. Expected value type: Vector3Int.\n",
                         getFigureName(figure));    //TODO: MESSAGE ERROR CAN BE IMPROVED
                return;
            }
            attrValue.vectorInt = vn->value.vectorInt;
            if(attrValue.vector != NULL)
                setFigureAttribute(figure, type, attrValue);
            break;
        case ATTR_CHILD:
            vn = getChildNode(valueNode, STRING_CONSTANT_NODE);
            if(vn == NULL) {
                logError(SYNTAX_ERROR, "Wrong attribute type in Figure: %s. Expected value type: String.\n",
                         getFigureName(figure));    //TODO: MESSAGE ERROR CAN BE IMPROVED
                return;
            }
            attrValue.figure = getFigureFromTable(vn -> value.string, context);
            if(attrValue.figure != NULL)
                setFigureAttribute(figure, type, attrValue);
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
    char * typeValue = getChildNode(getFigureAttributeValueNode(node, "type"), STRING_CONSTANT_NODE)->value.string;
    FigureType figureType = getFigureType(typeValue);
    if(figureType == FIGURE_INVALID){
        logError(SYNTAX_ERROR, "Invalid figure type %s\n", typeValue);
        return NULL;
    }

    logDebug("Generating figure: %s (Type: %s)\n", name, typeValue);
    Figure figure = newFigure(name, figureType);
    setAllAttributes(node, figure, context);

    printFigure(figure); //TODO: REMOVE

    return figure;
}

int parseFigureNode(Node * node, U3D_Context * context){
    Figure figure = generateFigure(node, context);
    addFigureToTable(figure, context);
    return 0;
}