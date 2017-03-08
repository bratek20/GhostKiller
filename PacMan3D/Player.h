#ifndef PLAYER_H
#define PLAYER_H

#include "ObjectsManager.h"
typedef struct ENEMY Enemy;

typedef struct PLAYER {
    Object* object;

    float cameraHeight;

    vec3 cameraPos;
    vec3 cameraFront;
    vec3 cameraUp;
    
    GLfloat yaw;
    GLfloat pitch;

    int canFly;

    vec3 force; // normalised force vector
    float duration; // duration of force impact
    
    float jumpHeight;
    int dJumpHeight;
} Player;


Player* makePlayer(float cameraHeight, int canFly);
void setPlayerPosition(Player* player, float x, float z);
void setPlayerCameraPosition(Player* player, vec3 pos);

void updatePlayer(Player* player, float delta);

void setPlayerForce(Player* player, vec3 force, float duration);

void makePlayerJump(Player* player);
void movePlayerUp(Player* player);
void movePlayerDown(Player* player);
void movePlayerLeft(Player* player);
void movePlayerRight(Player* player);

void rotatePlayerSight(Player* player, float xoffset, float yoffset);
void setPlayerPerspective(Player* player);

void playerShoot(Player* player);

void playerGameEndRotation(Player* player, Enemy* killer, float delta);

#endif
