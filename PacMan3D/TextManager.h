#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include "ObjectsManager.h"

typedef struct WAVE_DESCRIPTION {
    Object* wave;
    Object* number;
} WaveDescription;

WaveDescription* makeWaveDescription(float x, float z, float rotation);

void initTextManager();
void clearTextManager();

void drawStartScreen();
void drawLostScreen();
void drawWonScreen();

void drawWaveDescription(int waveNumber);

#endif