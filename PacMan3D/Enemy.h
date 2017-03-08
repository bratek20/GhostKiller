#ifndef ENEMY_H
#define ENEMY_H

#include "ObjectsManager.h"

typedef struct ENEMY {
    Object* head;
    Object* body;

    int isAlive;
    int life;
    vec3 force;
    float duration;

    float lifeTime;
} Enemy;

Enemy* makeEnemy();
void reinitEnemy(Enemy* enemy);

void updateEnemy(Enemy* enemy, float delta);

void setEnemyPosition(Enemy* enemy, float x, float z);
void setEnemyForce(Enemy* enemy, vec3 force, float duration);

int enemyCollideWithObject(Enemy* enemy, Object* object);

#endif
