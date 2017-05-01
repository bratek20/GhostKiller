#include "TextManager.h"
#include "Assets.h"
#include "ObjectsManager.h"

WaveDescription* makeWaveDescription(float x, float z, float rotation) {
    WaveDescription* wd = malloc(sizeof(WaveDescription));

    wd->wave = makeObject(SQUARE_SHAPE_2D, WAVE_TEXTURE, SCENE_WIDTH / 8, SCENE_WIDTH / 16, 1);
    wd->number = makeObject(SQUARE_SHAPE_2D, WAVE_NUMBER_TEXTURE[1], SCENE_WIDTH / 32, SCENE_WIDTH / 16, 1);

    setObjectPosition(wd->wave, x, WALL_HEIGHT + WAVE_DESCRIPTION_HEIGHT, z);
    setObjectPosition(wd->number, x - sign(z)*SCENE_WIDTH/8 , WALL_HEIGHT + WAVE_DESCRIPTION_HEIGHT, z + sign(x)*SCENE_WIDTH / 8);

    setObjectRotationY(wd->wave, rotation);
    setObjectRotationY(wd->number, rotation);
    
    return wd;
}

WaveDescription* waveDescriptions[4];
Object* startScreen;
Object* lostScreen;
Object* wonScreen;

void initTextManager() {
    waveDescriptions[0] = makeWaveDescription(0, -SCENE_DEPTH / 2, 0);
    waveDescriptions[1] = makeWaveDescription(SCENE_WIDTH / 2, 0, 90);
    waveDescriptions[2] = makeWaveDescription(0, SCENE_DEPTH / 2, 180); 
    waveDescriptions[3] = makeWaveDescription(-SCENE_WIDTH / 2, 0, 270);

    startScreen = makeObject(SQUARE_SHAPE_2D, START_SCREEN_TEXTURE, 2, 2, 2);
    lostScreen = makeObject(SQUARE_SHAPE_2D, LOST_SCREEN_TEXTURE, 2, 2, 2);
    wonScreen = makeObject(SQUARE_SHAPE_2D, WON_SCREEN_TEXTURE, 2, 2, 2);
}

void clearTextManager() {
    for (int i = 0; i < 4; i++)
        free(waveDescriptions[i]);

    free(startScreen);
    free(lostScreen);
    free(wonScreen);
}

void init2D(){
    GLuint program = GLOBAL_PROGRAM;
    glUseProgram(program);

    mat4x4 view;
    mat4x4 projection;
    mat4x4_identity(view); mat4x4_identity(projection);

    GLint viewLoc = glGetUniformLocation(program, "view");
    GLint projLoc = glGetUniformLocation(program, "projection");
    // Pass them to the shaders
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
}

void drawStartScreen() {
    init2D();
    drawObject(startScreen);
}

void drawLostScreen() {
    init2D();
    drawObject(lostScreen);
}

void drawWonScreen() {
    init2D();
    drawObject(wonScreen);
}

void drawWaveDescription(int waveNumber) {
    for (int i = 0; i < 4; i++) {
        waveDescriptions[i]->number->texture = WAVE_NUMBER_TEXTURE[waveNumber];
        drawObject(waveDescriptions[i]->wave);
        drawObject(waveDescriptions[i]->number);
    }
}
