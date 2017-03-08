#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H

typedef struct WAVE_INFO {
    int enemies;

    float spawnTime;
    int spawnAmount;
} WaveInfo;

typedef struct REWARD_INFO {
    int boxes;
    float height;
    int moveable;

    float extraTime;
} RewardInfo;

void updateWaveManager(float delta);

int getCurrentWave();
void initWaveManager();

#endif // !WAVE_MANAGER_H

