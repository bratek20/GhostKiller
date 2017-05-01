#ifndef BOX_H
#define BOX_H

#include "ObjectsManager.h"

typedef struct BOX {
    Object* object;
    int moveable;

    vec3 force;
    float duration;
} Box;

Box* makeBox(float x, float z, float height, int moveable);
Box* makeRandomBox(float height, int moveable);

void updateBox(Box* box, float delta);

void setBoxPosition(Box* box, float x, float z);
void setBoxForce(Box* box, vec3 force, float duration);

#endif
