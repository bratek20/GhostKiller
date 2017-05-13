#include "Assets.h"
#include "Shader.h"
#include "ObjectsManager.h"
#include "SOIL.h"

void loadAssets() {
    GLOBAL_PROGRAM = makeProgram(vertexCode3D, fragmentCode3D);

    loadTexture(&PLAIN_TEXTURE, "textures/plain.png");
    loadTexture(&PLAIN_WITH_EDGES_TEXTURE, "textures/plainWithEdges.png");
    loadTexture(&WOODEN_BOX_TEXTURE, "textures/woodenBox.jpg");
    loadTexture(&METAL_BOX_TEXTURE, "textures/metalBox.jpg");
    loadTexture(&ROCK_TEXTURE, "textures/rock.jpg");
    loadTexture(&METAL_TEXTURE, "textures/metal.jpg");
    
    loadTexture(&GHOST_HEAD_TEXTURE, "textures/ghostHead.jpg");
    loadTexture(&GHOST_BODY_TEXTURE, "textures/ghostBody.jpeg");

    loadSQUARE_SHAPE();
    loadSQUARE_SHAPE_2D();
    loadCUBE_SHAPE();
    loadText();
}

void disposeAssets() {
    deleteShape(SQUARE_SHAPE);
    deleteShape(SQUARE_SHAPE_2D);
    deleteShape(CUBE_SHAPE);

    glDeleteTextures(1, &PLAIN_TEXTURE);
    glDeleteTextures(1, &PLAIN_WITH_EDGES_TEXTURE);
    glDeleteTextures(1, &ROCK_TEXTURE);
    glDeleteTextures(1, &METAL_TEXTURE);
    glDeleteTextures(1, &WOODEN_BOX_TEXTURE);
    glDeleteTextures(1, &METAL_BOX_TEXTURE);
    glDeleteTextures(1, &GHOST_BODY_TEXTURE);
    glDeleteTextures(1, &GHOST_HEAD_TEXTURE);

    glDeleteTextures(1, &WAVE_TEXTURE);
    for (int i = 1; i <= LAST_WAVE; i++) {
        glDeleteTextures(1, &WAVE_NUMBER_TEXTURE[i]);
    }
    
    glDeleteTextures(1, &START_SCREEN_TEXTURE);
    glDeleteTextures(1, &LOST_SCREEN_TEXTURE);
    glDeleteTextures(1, &WON_SCREEN_TEXTURE);

    glUseProgram(0);
    glDeleteProgram(GLOBAL_PROGRAM);
}

void loadText() {
    loadTexture(&WAVE_TEXTURE, "textures/wave.png");
    char name[] = "textures/waveNumber0.png";
    for (int i = 1; i <= LAST_WAVE; i++) {
        name[19] = i + '0';
        loadTexture(&WAVE_NUMBER_TEXTURE[i], name);
    }

    loadTexture(&START_SCREEN_TEXTURE, "textures/startScreen.png");
    loadTexture(&LOST_SCREEN_TEXTURE, "textures/lostScreen.png");
    loadTexture(&WON_SCREEN_TEXTURE, "textures/wonScreen.png");
}

void loadTexture(GLuint* name, char* path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); 

    (*name) = texture;
}

void loadCUBE_SHAPE() {
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

        0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f
    };
    vec3 size = { 1,1,1 };

    GLuint VAO = 0;
    loadVertices(vertices, sizeof(vertices), &VAO);
    
    CUBE_SHAPE = makeShape(VAO, 36, size);
}

void loadSQUARE_SHAPE() {
    GLfloat vertices[] = {
        -1.0, 0.0, -1.0,  0.0, 0.0,
        -1.0, 0.0, 1.0,  0.0, 1.0,
        1.0, 0.0, 1.0,  1.0, 1.0,

        
        1.0, 0.0, 1.0, 1.0, 1.0,
        1.0, 0.0, -1.0, 1.0, 0.0,
	-1.0, 0.0, -1.0, 0.0, 0.0,
    };

    vec3 size = { 2,1,2 };

    GLuint VAO = 0;
    loadVertices(vertices, sizeof(vertices), &VAO);

    SQUARE_SHAPE = makeShape(VAO, 6, size);
}

void loadSQUARE_SHAPE_2D() {
    GLfloat vertices[] = {
        -1.0, -1.0, 0.0, 0.0, 0.0,
        -1.0, 1.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 0.0,1.0, 1.0,

	1.0, 1.0, 0.0, 1.0, 1.0,    
	1.0, -1.0, 0.0, 1.0, 0.0,
	-1.0, -1.0, 0.0, 0.0, 0.0,
        
    };

    vec3 size = { 2,2,1 };

    GLuint VAO = 0;
    loadVertices(vertices, sizeof(vertices), &VAO);

    SQUARE_SHAPE_2D = makeShape(VAO, 6, size);
}

void loadVertices(GLfloat* vertices, GLsizeiptr size, GLuint* VAO) {
    glGenVertexArrays(1, VAO);
    
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);

    glBindVertexArray((*VAO));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}
