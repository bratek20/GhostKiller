#ifndef OBJECTS_MANAGER_H
#define OBJECTS_MANAGER_H

#include <GL/glew.h>
#include "linmath.h"
#include "defines.h"

typedef struct SHAPE {
    GLuint VAO;
    GLuint triangles;    
    vec3 size; // width, height, depth
} Shape;

typedef struct OBJECT {
    Shape* shape;
    GLuint texture;

    vec3 position;
    vec2 textScale; // how texture should be scaled
    vec3 scale;
    vec3 rotation;
    vec4 color;

    float width;
    float height;
    float depth;
} Object;

Shape* makeShape(GLuint VAO, GLuint triangles, vec3 size);
void deleteShape(Shape* shape);

Object* makeObject(Shape* shape, GLuint texture, float width, float height, float depth);
Object* makeObjectWithTextScaled(Shape* shape, GLuint texture, float width, float height, float depth, float textScaleX, float textScaleY);

void setObjectPosition(Object* object, float x, float y, float z);
void setObjectColor(Object* object, float r, float g, float b, float a);
void setObjectRotationX(Object* object, float angle);
void setObjectRotationY(Object* object, float angle);
void setObjectRotationZ(Object* object, float angle);

void drawObject(Object* object);

void updateObjectPosition(Object* object, vec3 force, float delta);

int objectContainsPoint(Object* object, vec3 point);
int objectContainsObject(Object* tester, Object* tested);
int objectsCollide(Object* o1, Object* o2);

#endif
