#include "Enemy.h"
#include "Assets.h"
#include "Scene.h"
#include "Player.h"

Enemy* makeEnemy() {
    Enemy* enemy = malloc(sizeof(Enemy));
    
    enemy->body = makeObject(CUBE_SHAPE, GHOST_BODY_TEXTURE, 0.8, 1.2, 0.8);
    setObjectColor(enemy->body, 1, 1, 1, 0.5);

    enemy->head = makeObject(CUBE_SHAPE, GHOST_HEAD_TEXTURE, 0.3, 0.3, 0.3);
    setObjectColor(enemy->head, 1, 1, 1, 0.6);   
    
    reinitEnemy(enemy);
    
    return enemy;
}

void reinitEnemy(Enemy* enemy) {
    float x = 0, z = 0;
    vec3 zero = { 0,0,0 };
    memcpy(enemy->force, zero, sizeof(vec3));
    
    do {
        int side = rand() % 2;
        if (side == 0) {
            x = SCENE_WIDTH / 2 * 1.1;
            z = rand() % ((int)SCENE_DEPTH / 2);
        }
        else {
            x = rand() % ((int)SCENE_WIDTH / 2);
            z = SCENE_DEPTH / 2 * 1.1;
        }

        int sign = rand() % 2;
        if (sign == 0) x *= -1;
        sign = rand() % 2;
        if (sign == 0) z *= -1;

        setEnemyPosition(enemy, x, z);
    } while (makeCollisionEnemyEnemies(enemy)); // do not spawn enemies in enemies

    enemy->duration = -1;
    enemy->isAlive = 1;
    enemy->life = ENEMY_LIFE;
    enemy->lifeTime = 0;
}

void setEnemyPosition(Enemy* enemy, float x, float z) {
    float over = sinf(enemy->lifeTime*ENEMY_FLUCTUATION_SPEED) * ENEMY_FLUCTUATION_RANGE; // fluctuation
    
    over += ENEMY_ABOVE + enemy->body->height / 2;
    setObjectPosition(enemy->body, x,  over, z);
    
    over += enemy->body->height / 2 + ENEMY_HEAD_ABOVE + enemy->head->height / 2;
    setObjectPosition(enemy->head, x, over, z);
}

int enemyCollideWithObject(Enemy* enemy, Object* object) {
    if (!enemy->isAlive)
        return 0;
    return objectsCollide(enemy->head, object) || objectsCollide(enemy->body, object);
}

void enemyLookForPlayer(Enemy* enemy) {
    Player* player = GLOBAL_PLAYER;
    vec3 dPos = { player->cameraPos[0] - enemy->body->position[0], 0, player->cameraPos[2] - enemy->body->position[2] };

    vec3_norm(dPos, dPos);
    setEnemyForce(enemy, dPos, ENEMY_CHASE_DURATION);
    
    vec3 norm = { 1,0,0 };
    if (enemy->force[2] < 0) norm[0] = -1;
    float angle = toDegree(acosf(vec3_mul_inner(norm, enemy->force)));

    setObjectRotationY(enemy->head, angle);
    setObjectRotationY(enemy->body, angle);
}

void updateEnemy(Enemy* enemy, float delta) {
    if (enemy->life <= 0) {
        enemy->isAlive = 0;
    }
    
    if (!enemy->isAlive) return;
    
    enemy->lifeTime += delta;
    enemy->duration -= delta;
    if (enemy->duration < 0) {
        enemyLookForPlayer(enemy);
    }

    vec3 oldPos; memcpy(oldPos, enemy->body->position, sizeof(vec3));
    updateObjectPosition(enemy->body, enemy->force, delta*ENEMY_SPEED);
    if (enemyCollideWithObject(enemy, GLOBAL_PLAYER->object)) {
        startGameEndInScene(enemy);
        return;
    }
    
    if ( makeCollisionEnemyBoxes(enemy) ) {
        memcpy(enemy->body->position, oldPos, sizeof(vec3));
        mat4x4 rotation; mat4x4_identity(rotation);
        mat4x4_rotate_Y(rotation, rotation, asDegree(90+45));
        vec4 newForce = { 0,0,0,1 };
        for (int i = 0; i < 3; i++) newForce[i] = enemy->force[i];
        
        vec4 res;
        mat4x4_mul_vec4(res, rotation, newForce);
        for (int i = 0; i < 3; i++) enemy->force[i] = res[i];
        
        setEnemyForce(enemy, enemy->force, ENEMY_CHASE_DURATION);
    }

    /*if (makeCollisionEnemyEnemies(enemy)) {
        memcpy(enemy->body->position, oldPos, sizeof(vec3));
    }*/

    setEnemyPosition(enemy, enemy->body->position[0], enemy->body->position[2]);
}

void setEnemyForce(Enemy* enemy, vec3 force, float duration) {
    memcpy(enemy->force, force, sizeof(vec3));
    enemy->duration = duration;
}