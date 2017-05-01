#include "Bullet.h"
#include "Player.h"
#include "Assets.h"

Bullet* makeBullet(Player* shooter) {
    Bullet* bullet = malloc(sizeof(Bullet));
    
    bullet->object = makeObject(CUBE_SHAPE, PLAIN_TEXTURE, 0.05, 0.05, 0.05);
    setObjectColor(bullet->object, 1, 0, 0, 0.3f);

    reinitBullet(bullet, shooter);

    return bullet;
}

void reinitBullet(Bullet* bullet, Player* shooter) {
    memcpy(bullet->dir, shooter->cameraFront, sizeof(vec3));
    bullet->isFlying = 1;

    setObjectPosition(bullet->object, shooter->cameraPos[0], shooter->cameraPos[1], shooter->cameraPos[2]);
}

void updateBullet(Bullet* bullet, float delta) {
    vec3 dPos;
    vec3_scale(dPos, bullet->dir, BULLET_SPEED*delta);
    vec3_add(bullet->object->position, bullet->object->position, dPos);
}