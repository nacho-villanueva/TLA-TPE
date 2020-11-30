#include <stdlib.h>
#include <math.h>

#include "vector3.h"
#include "../utils/logger.h"

#define EPSILON 0.0001

Vector3 newVector3(float x, float y, float z){
    Vector3 vector = malloc(sizeof(struct vector3));
    if(vector == NULL){
        logError(ERROR, "vector3.c: Cannot allocate memory\n");
    }
    vector -> x = x;
    vector -> y = y;
    vector -> z = z;
    return vector;
}

Vector3Int newVector3Int(int x, int y, int z){
    Vector3Int vector = malloc(sizeof(struct vector3Int));
    if(vector == NULL){
        logError(ERROR, "vector3.c: Cannot allocate memory\n");
    }
    vector -> x = x;
    vector -> y = y;
    vector -> z = z;
    return vector;
}

Vector3 vector3IntToVector3(Vector3Int vector3Int) {
    Vector3 vector3 = newVector3((float)vector3Int->x,(float)vector3Int->y,(float)vector3Int->z);
    free(vector3Int);
    return vector3;
}

Vector3Int vector3ToVector3Int(Vector3 vector3){
    Vector3Int vector3Int = newVector3Int((int)vector3->x, (int)vector3->y, (int)vector3->z);
    free(vector3);
    return vector3Int;
}

void addVector3(Vector3 vectorA, Vector3 vectorB, Vector3 result){
    result -> x = vectorA -> x + vectorB -> x;
    result -> y = vectorA -> y + vectorB -> y;
    result -> z = vectorA -> z + vectorB -> z;
}

void multiplyVector3(Vector3 vector, float mul, Vector3 result){
    result -> x = vector -> x * mul;
    result -> y = vector -> y * mul;
    result -> z = vector -> z * mul;
}

bool equalsVector3(Vector3 vectorA, Vector3 vectorB){
    return distanceVector3(vectorA, vectorB) < EPSILON;
}


float distanceVector3(Vector3 vectorA, Vector3 vectorB){
    float difX = vectorB -> x - vectorA -> x;
    float difY = vectorB -> y - vectorA -> y;
    float difZ = vectorB -> z - vectorA -> z;

    difX *= difX;
    difY *= difY;
    difZ *= difZ;

    return (float)sqrt((double)difX + difY + difZ);
}

void printVector3Int(Vector3Int vector3Int) {
    logInfo("(%d, %d, %d)", vector3Int->x, vector3Int->y, vector3Int->z);
}

void printVector3(Vector3 vector3) {
    logInfo("(%.2f, %.2f, %.2f)", vector3->x, vector3->y, vector3->z);
}
