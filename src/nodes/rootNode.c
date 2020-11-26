#include "rootNode.h"
#include <stdio.h>
#include "../utils/parser.h"

int parseRootNode(Node * node, U3D_Context *  context){
    int ret = 0;
    //parse("float x, y;float dim = 80.0;void setup() {size(640, 360);noStroke();}void draw() {background(102);x = x + 0.8;if (x > width + dim) {x = -dim;} translate(x, height/2-dim/2);fill(255);rect(-dim/2, -dim/2, dim, dim);translate(x, dim);fill(0);rect(-dim/2, -dim/2, dim, dim);}");
    for(int i = 0; i < node->childrenCount; i++){
        if(node->children[i] != NULL)
            ret += parseNode(node->children[i], context);
    }
    return ret;
}