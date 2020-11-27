#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <stdbool.h>

struct vector3 {
    float x;
    float y;
    float z;
};

typedef struct vector3 * Vector3;

struct vector3Int {
    int x;
    int y;
    int z;
};

typedef struct vector3Int * Vector3Int;

Vector3 newVector3(float x, float y, float z);
Vector3Int newVector3Int(int x, int y, int z);

void addVector3(Vector3 vectorA, Vector3 vectorB, Vector3 result);
void multiplyVector3(Vector3 vector, float mul, Vector3 result);

bool equalsVector3(Vector3 vectorA, Vector3 vectorB);
float distanceVector3(Vector3 vectorA, Vector3 vectorB);
#endif /* _VECTOR3_H_ */
