#include "WaveManager.h"
#include "Scene.h"
#include "defines.h"

WaveInfo waveInfos[] = {
    {0,0,0}, // wave nr 0 doesnt exsist
    {10, 5, 2},  // number of enemies , time in sec to spawn subwave, subwave number of enemies
    {20, 10, 4},
    {40, 7.5, 4},
    {80, 7.5, 5},
    {100, 10, 10}
};

RewardInfo rewards[] = { // rewards for completing wave
    {0,0,0,0},
    {1, 2, 0, 5}, // boxes, height, moveable, extra time
    {2, 2, 0, 10},
    {4, 1.5, 0, 15},
    {2, 1.0, 1, 25}
};

int currentWave;
int getCurrentWave() {
    return currentWave;
}

void spawnEnemies(int amount) {
    while (amount--) {
        addEnemyToScene();
    }
}

void spawnRewardBoxes(int amount, float height, int moveable) {
    while (amount--) {
        addRandomBoxToScene(height, moveable);
    }
}

float currentTime;
float timeToStart;
int spawnedEnemies;
void initNewWave() {
    currentTime = 0;
    spawnedEnemies = 0;
    timeToStart = NEXT_WAVE_WAIT_TIME;
}

void updateWaveManager(float delta) {
    if (timeToStart > 0) {
        timeToStart -= delta;
        return;
    }

    if (spawnedEnemies < waveInfos[currentWave].enemies) {
        currentTime += delta;
        if (currentTime > waveInfos[currentWave].spawnTime) {
            currentTime = 0;
            spawnEnemies(waveInfos[currentWave].spawnAmount);

            spawnedEnemies += waveInfos[currentWave].spawnAmount;
        }
    }
    else if (countAliveEnemies() == 0) {
        if (currentWave == LAST_WAVE) {
            startGameEndInScene(NULL);
            return;
        }

        initNewWave();
        timeToStart += rewards[currentWave].extraTime;
        spawnRewardBoxes(rewards[currentWave].boxes, rewards[currentWave].height, rewards[currentWave].moveable);
        currentWave++;
    }
}

void initWaveManager() {
    currentWave = 1;
    initNewWave();

    addBoxToScene(SCENE_WIDTH / 4, SCENE_DEPTH / 4, 3, 0);
    addBoxToScene(-SCENE_WIDTH / 4, SCENE_DEPTH / 4, 3, 0);
    addBoxToScene(SCENE_WIDTH / 4, -SCENE_DEPTH / 4, 3, 0);
    addBoxToScene(-SCENE_WIDTH / 4, -SCENE_DEPTH / 4, 3, 0);

    addRandomBoxToScene(1, 1);
    addRandomBoxToScene(1.8, 0);

    // God protection :D
    /*addBoxToScene(2, 0, 1, 1);
    addBoxToScene(-2, 0, 1, 1);
    addBoxToScene(0, 2, 1, 1);
    addBoxToScene(0, -2, 1, 1);*/
}