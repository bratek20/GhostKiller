#ifndef BULLET_H
#define BULLET_H

#include "ObjectsManager.h"
typedef struct PLAYER Player;

typedef struct BULLET {
    Object* object;

    int isFlying;
    vec3 dir;
} Bullet;

Bullet* makeBullet(Player* shooter);
void reinitBullet(Bullet* bullet, Player* shooter);

void updateBullet(Bullet* bullet, float delta);

#endif
