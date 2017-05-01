#ifndef ASSETS_H
#define ASSETS_H

#include <GL/glew.h>
#include "defines.h"

typedef struct SHAPE Shape;

GLuint GLOBAL_PROGRAM;

Shape* CUBE_SHAPE;
Shape* SQUARE_SHAPE;
Shape* SQUARE_SHAPE_2D;

GLuint PLAIN_TEXTURE;
GLuint PLAIN_WITH_EDGES_TEXTURE;
GLuint WOODEN_BOX_TEXTURE;
GLuint METAL_BOX_TEXTURE;
GLuint ROCK_TEXTURE;
GLuint METAL_TEXTURE;

GLuint GHOST_HEAD_TEXTURE;
GLuint GHOST_BODY_TEXTURE;

GLuint START_SCREEN_TEXTURE;
GLuint LOST_SCREEN_TEXTURE;
GLuint WON_SCREEN_TEXTURE;
GLuint WAVE_TEXTURE;
GLuint WAVE_NUMBER_TEXTURE[LAST_WAVE+1];

void loadAssets();
void loadTexture(GLuint* name, char* path);
void loadVertices(GLfloat* vertices, GLsizeiptr size, GLuint* VAO);
void loadCUBE_SHAPE();
void loadSQUARE_SHAPE();
void loadSQUARE_SHAPE_2D();
void loadText();
void disposeAssets();

#endif // ASSETS_H

