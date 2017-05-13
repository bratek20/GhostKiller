#include "ObjectsManager.h"
#include "Assets.h"
#include <stdlib.h>

Shape* makeShape(GLuint VAO, GLuint triangles, vec3 size) {
    Shape* shape = malloc(sizeof(Shape));
    
    shape->VAO = VAO;
    shape->triangles = triangles;
    memcpy(shape->size, size, sizeof(vec3));

    return shape;
}

void deleteShape(Shape* shape) {
    glDeleteVertexArrays(1, &shape->VAO);
    free(shape);
}

Object* makeObject(Shape* shape, GLuint texture, float width, float height, float depth) {
    Object* object = malloc(sizeof(Object));

    object->shape = shape;
    object->texture = texture;

    object->width = width;
    object->height = height;
    object->depth = depth;

    vec3 zero = { 0,0,0 };
    vec3 one = { 1,1,1 };

    memcpy(object->position, zero, sizeof(vec3));
    memcpy(object->rotation, zero, sizeof(vec3));
    memcpy(object->textScale, one, sizeof(vec2));
    setObjectColor(object, 1, 1, 1, 1);

    vec3 scale = { width / shape->size[0], height / shape->size[1], depth / shape->size[2] };
    memcpy(object->scale, scale, sizeof(vec3));

    return object;
}

Object* makeObjectWithTextScaled(Shape* shape, GLuint texture, float width, float height, float depth, float textScaleX, float textScaleY) {
    Object* object = makeObject(shape, texture, width, height, depth);
    
    object->textScale[0] = textScaleX;
    object->textScale[1] = textScaleY;

    return object;
}

void setObjectPosition(Object* object, float x, float y, float z) {
    object->position[0] = x;
    object->position[1] = y;
    object->position[2] = z;
}

void setObjectColor(Object* object, float r, float g, float b, float a) {
    object->color[0] = r;
    object->color[1] = g;
    object->color[2] = b;
    object->color[3] = a;
}

void setObjectRotationX(Object* object, float angle) {
    object->rotation[0] = angle;
}

void setObjectRotationY(Object* object, float angle) {
    object->rotation[1] = angle;
}

void setObjectRotationZ(Object* object, float angle) {
    object->rotation[2] = angle;
}

void drawObject(Object* object) {
    GLuint VAO = object->shape->VAO;
    GLuint triangles = object->shape->triangles;

    glBindVertexArray(VAO);
        
    mat4x4 model;

    mat4x4_translate(model, object->position[0], object->position[1], object->position[2]);

    mat4x4_rotate_Z(model, model, asDegree(object->rotation[2]));
    mat4x4_rotate_Y(model, model, asDegree(object->rotation[1]));
    mat4x4_rotate_X(model, model, asDegree(object->rotation[0]));  

    mat4x4_scale_aniso(model, model, object->scale[0], object->scale[1], object->scale[2]);

    glBindTexture(GL_TEXTURE_2D, object->texture);

    GLint modelLoc = glGetUniformLocation(GLOBAL_PROGRAM, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *)model);
    
    GLint colorLoc = glGetUniformLocation(GLOBAL_PROGRAM, "color");
    GLint coordScaleLoc = glGetUniformLocation(GLOBAL_PROGRAM, "coordScale");
    glUniform2f(coordScaleLoc, object->textScale[0], object->textScale[1]);

    glUniform4f(colorLoc, object->color[0], object->color[1], object->color[2], object->color[3]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, triangles);
    
    glBindVertexArray(0);
}

void updateObjectPosition(Object* object, vec3 force, float delta) {
    vec3 tmp;
    vec3_scale(tmp, force, delta*OBJECT_SHIFT);
    vec3_add(object->position, object->position, tmp);
}

int objectContainsPoint(Object* object, vec3 point) {
    vec3 minP = { object->position[0] - object->width/2, 
                  object->position[1] - object->height/2,
                  object->position[2] - object->depth/2 };
    vec3 maxP = { object->position[0] + object->width/2,
        object->position[1] + object->height/2,
        object->position[2] + object->depth/2 };

    for (int i = 0; i < 3; i++)
        if (minP[i] > point[i] || maxP[i] < point[i])
            return 0;
    return 1;
}

int dx[] = {-1,-1,1,1,-1,-1,1,1 };
int dy[] = {-1,-1,-1,-1,1,1,1,1};
int dz[] = {-1,1,-1,1,-1,1,-1,1 };
int objectContainsObject(Object* tester, Object* tested) {
    vec3 pos; memcpy(pos, tested->position, sizeof(vec3));
    
    float hw = tested->width / 2; // half width
    float hh = tested->height / 2;
    float hd = tested->depth / 2;
    
    for (int i = 0; i < 8; i++) {
        vec3 point = { pos[0] + dx[i] * hw, pos[1] + dy[i] * hh, pos[2] + dz[i] * hd };
        if (objectContainsPoint(tester, point))
            return 1;
    }
    return 0;
}

int objectsCollide(Object* o1, Object* o2) {
    return objectContainsObject(o1, o2) || objectContainsObject(o2, o1);
}
