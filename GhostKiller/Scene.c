
#include "Scene.h"
#include "Assets.h"
#include "Player.h"
#include "ViewFinder.h"
#include "Box.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Shader.h"
#include "TextManager.h"
#include "WaveManager.h"

Object* board[5];
int boxesIdx = 0;
Box* boxes[MAX_BOXES];
int enemiesIdx = 0;
Enemy* enemies[MAX_ENEMIES];
int bulletsIdx = 0;
Bullet* bullets[MAX_BULLETS];

int gameBeforeEnd = 1;
int gameEnded = 1;
int lastSceneStatus;

void initScene() {
    lastSceneStatus = NO_STATUS;

    GLOBAL_PLAYER = makePlayer(1.8, 0);
    GLOBAL_VIEW_FINDER = makeViewFinder();

    board[0] = makeObjectWithTextScaled(SQUARE_SHAPE, ROCK_TEXTURE, SCENE_WIDTH, 0, SCENE_DEPTH, SCENE_WIDTH / 5, SCENE_DEPTH / 5);
    setObjectPosition(board[0], 0, BOARD_FALL, 0);
    setObjectColor(board[0], 0.5, 0.5, 0.5, 1);

    board[1] = makeObjectWithTextScaled(SQUARE_SHAPE, ROCK_TEXTURE, SCENE_WIDTH, 0, WALL_HEIGHT, SCENE_WIDTH / 10, WALL_HEIGHT / 10);
    setObjectPosition(board[1], 0, BOARD_FALL+WALL_HEIGHT/2, SCENE_DEPTH / 2);
    setObjectRotationX(board[1], 90);

    board[2] = makeObjectWithTextScaled(SQUARE_SHAPE, ROCK_TEXTURE, SCENE_WIDTH, 0, WALL_HEIGHT, SCENE_WIDTH / 10, WALL_HEIGHT / 10);
    setObjectPosition(board[2], 0, BOARD_FALL + WALL_HEIGHT / 2, -SCENE_DEPTH / 2);
    setObjectRotationX(board[2], 90);

    board[3] = makeObjectWithTextScaled(SQUARE_SHAPE, ROCK_TEXTURE, SCENE_WIDTH, 0, WALL_HEIGHT, SCENE_WIDTH / 10, (float)WALL_HEIGHT / 10);
    setObjectPosition(board[3], SCENE_WIDTH / 2, BOARD_FALL + WALL_HEIGHT / 2, 0);
    setObjectRotationX(board[3], 90);
    setObjectRotationY(board[3], 90);

    board[4] = makeObjectWithTextScaled(SQUARE_SHAPE, ROCK_TEXTURE, SCENE_WIDTH, 0, WALL_HEIGHT, SCENE_WIDTH / 10, (float)WALL_HEIGHT / 10);
    setObjectPosition(board[4], -SCENE_WIDTH / 2, BOARD_FALL + WALL_HEIGHT / 2, 0);
    setObjectRotationX(board[4], 90);
    setObjectRotationY(board[4], 90);
}

void clearScene() {
    free(GLOBAL_PLAYER);
    free(GLOBAL_VIEW_FINDER);

    for (int i = 0; i < 5; i++)
        free(board[i]);
    for (int i = 0; i < bulletsIdx; i++)
        free(bullets[i]);
    for (int i = 0; i < enemiesIdx; i++)
        free(enemies[i]);
    for (int i = 0; i < boxesIdx; i++)
        free(boxes[i]);

    bulletsIdx = enemiesIdx = boxesIdx = 0;
}

void startGameInScene() {
    setPlayerPosition(GLOBAL_PLAYER,0,0);
    
    for (int i = 0; i < bulletsIdx; i++)
        free(bullets[i]);
    for (int i = 0; i < enemiesIdx; i++)
        free(enemies[i]);
    for (int i = 0; i < boxesIdx; i++)
        free(boxes[i]);

    bulletsIdx = enemiesIdx = boxesIdx = 0;

    gameBeforeEnd = 0;
    gameEnded = 0;
    initWaveManager();
}

int getLastSceneStatus() {
    return lastSceneStatus;
}
Enemy* PLAYER_KILLER;
void startGameEndInScene(Enemy* killer) {
    if (killer == NULL) {
        lastSceneStatus = WON_STATUS;
    }
    else {
        lastSceneStatus = LOST_STATUS;
    }

    PLAYER_KILLER = killer;
    gameBeforeEnd = 1;
}

void endGameInScene() {
    gameEnded = 1;
}

int isGameBeforeEndInScene() {
    return gameBeforeEnd;
}

int isGameEndedInScene() {
    return gameEnded;
}

void addBoxToScene(float x, float z, float height, int moveable) {
    if (MAX_BOXES <= boxesIdx) {
        return;
    }

    boxes[boxesIdx] = makeBox(x, z, height, moveable);
    boxesIdx++;
}

void addRandomBoxToScene(float height, int moveable) {
    if (MAX_BOXES <= boxesIdx) {
        return;
    }

    boxes[boxesIdx] = makeRandomBox(height, moveable);
    boxesIdx++;
}

void addEnemyToScene() {
    int idx = findDeadEnemy();
    if (idx != -1) {
        reinitEnemy(enemies[idx]);
        return;
    }

    if (MAX_ENEMIES <= enemiesIdx) {
        return;
    }
    enemies[enemiesIdx] = makeEnemy();
    enemiesIdx++;
}

int findDeadEnemy() {
    for (int i = 0; i < enemiesIdx; i++)
        if (!enemies[i]->isAlive)
            return i;
    return -1;
}

int countAliveEnemies() {
    int cnt = 0;
    for (int i = 0; i < enemiesIdx; i++)
        if (enemies[i]->isAlive)
            cnt++;
    return cnt;
}

void addBulletToScene(Player* shooter) {
    int idx = findNotFlyingBullet();
    if (idx != -1) {
        reinitBullet(bullets[idx], shooter);
    }

    if (MAX_BULLETS <= bulletsIdx) {
        return;
    }
    
    bullets[bulletsIdx] = makeBullet(shooter);
    bulletsIdx++;
}

int findNotFlyingBullet() {
    for (int i = 0; i < bulletsIdx; i++)
        if (!bullets[i]->isFlying)
            return i;
    return -1;
}

void updateScene(float delta) {
    if (gameBeforeEnd) {
        playerGameEndRotation(GLOBAL_PLAYER, PLAYER_KILLER, delta);
        return;
    }
    
    updateWaveManager(delta);

    updatePlayer(GLOBAL_PLAYER, delta);

    for (int i = 0; i < boxesIdx; i++)
        updateBox(boxes[i], delta);
    
    for (int i = 0; i < enemiesIdx; i++)
        updateEnemy(enemies[i], delta);

    for (int i = 0; i < bulletsIdx; i++) {
        if (!bullets[i]->isFlying) continue;

        updateBullet(bullets[i], delta);
        if (isObjectOutOfScene(bullets[i]->object)) {
            bullets[i]->isFlying = 0;
            continue;
        }

        if (makeCollisionBulletBoxes(bullets[i]) || makeCollisionBulletEnemies(bullets[i])) {
            continue;
        }
    }
}

void drawScene() {
    useProgram(GLOBAL_PROGRAM);

    drawViewFinder(GLOBAL_VIEW_FINDER);

    setPlayerPerspective(GLOBAL_PLAYER);

    drawWaveDescription(getCurrentWave());

    for (int i = 0; i < 5; i++)
        drawObject(board[i]);

    //drawObject(GLOBAL_PLAYER->object);

    for (int i = 0; i < boxesIdx; i++)
        drawObject(boxes[i]->object);

    for (int i = 0; i < enemiesIdx; i++)
        if (enemies[i]->isAlive) {
            drawObject(enemies[i]->head);
            drawObject(enemies[i]->body);
        }

    for (int i = 0; i < bulletsIdx; i++)
        if(bullets[i]->isFlying)
            drawObject(bullets[i]->object);
}

int isObjectOutOfScene(Object* object) {
    if (object->position[0] - object->width / 2 < -SCENE_WIDTH / 2) 
        return 1;
    if (object->position[0] + object->width / 2 > SCENE_WIDTH / 2)
        return 1;
    if (object->position[2] - object->depth / 2 < -SCENE_DEPTH / 2)
        return 1;
    if (object->position[2] + object->depth / 2 > SCENE_DEPTH / 2)
        return 1;

    return 0;
}

int makeCollisionPlayerBoxes(Player* player) {
    int collide = 0;
    for(int i=0;i<boxesIdx;i++)
        if (objectsCollide(player->object, boxes[i]->object)) {
            if (boxes[i]->moveable) {
                vec3 tmp; memcpy(tmp, player->force, sizeof(vec3));
                tmp[1] = 0; vec3_norm(tmp, tmp);

                setBoxForce(boxes[i], tmp, MOVE_DURATION);
            }
            collide = 1;
        }

    return collide;
}

int makeCollisionPlayerEnemies(Player* player) {
    int collide = 0;
    for (int i = 0; i<enemiesIdx; i++)
        if (enemyCollideWithObject(enemies[i], player->object)) {
            vec3 tmp; memcpy(tmp, player->cameraFront, sizeof(vec3));
            tmp[1] = 0; vec3_norm(tmp, tmp);

            setEnemyForce(enemies[i], tmp, MOVE_DURATION);
            collide = 1;
        }

    return collide;
}

int makeCollisionBoxEnemies(Box* box) {
    int collide = 0;
    for (int i = 0; i<enemiesIdx; i++)
        if (enemyCollideWithObject(enemies[i], box->object)) {
            vec3 tmp; memcpy(tmp, box->force, sizeof(vec3));
            tmp[1] = 0; vec3_norm(tmp, tmp);

            setEnemyForce(enemies[i], tmp, MOVE_DURATION);
            collide = 1;
        }

    return collide;
}

int makeCollisionBoxBoxes(Box* box) {
    int collide = 0;
    for (int i = 0; i<boxesIdx; i++)
        if (box != boxes[i] && objectsCollide(box->object, boxes[i]->object)) {
            if (boxes[i]->moveable) {
                setBoxForce(boxes[i], box->force, MOVE_DURATION);
            }
            collide = 1;
        }

    return collide;
}

int makeCollisionBulletBoxes(Bullet* bullet) {
    for (int i = 0; i < boxesIdx; i++) {
        if (bullet->isFlying && objectsCollide(bullet->object, boxes[i]->object)) {
            if (boxes[i]->moveable) {
                bullet->dir[1] = 0; vec3_norm(bullet->dir, bullet->dir);
                setBoxForce(boxes[i], bullet->dir, MOVE_BY_BULLET_DURATION);
            }
            bullet->isFlying = 0;
            
            return 1;
        }
    }
    return 0;
}

int makeCollisionBulletEnemies(Bullet* bullet) {
    for (int i = 0; i < enemiesIdx; i++) {
        if (bullet->isFlying && enemyCollideWithObject(enemies[i], bullet->object)) {
            if (objectsCollide(bullet->object, enemies[i]->head))
                enemies[i]->life -= HEAD_DMG;

            if (objectsCollide(bullet->object, enemies[i]->body))
                enemies[i]->life -= BODY_DMG;
            
            bullet->dir[1] = 0; vec3_norm(bullet->dir, bullet->dir);
            setEnemyForce(enemies[i], bullet->dir, MOVE_BY_BULLET_DURATION);
            bullet->isFlying = 0;

            return 1;
        }
    }
    return 0;
}

int makeCollisionEnemyBoxes(Enemy* enemy) {
    int collide = 0;
    for (int i = 0; i<boxesIdx; i++)
        if (objectsCollide(enemy->body, boxes[i]->object)) {
            vec3 tmp; memcpy(tmp, enemy->force, sizeof(vec3));
            tmp[1] = 0; vec3_norm(tmp, tmp);

            //setBoxForce(boxes[i], tmp, MOVE_DURATION);
            collide = 1;
        }

    return collide;
}

int makeCollisionEnemyEnemies(Enemy* enemy) {
    int collide = 0;
    for (int i = 0; i < enemiesIdx; i++)
        if ( enemy != enemies[i] && enemyCollideWithObject(enemies[i], enemy->body)) {
            vec3 tmp; memcpy(tmp, enemy->force, sizeof(vec3));
            if (enemy->force[0] != 0 || enemy->force[2] != 0) {
                tmp[1] = 0; vec3_norm(tmp, tmp);
            }

            setEnemyForce(enemies[i], tmp, MOVE_DURATION);
            collide = 1;
        }

    return collide;
}
