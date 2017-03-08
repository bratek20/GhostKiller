#include "Player.h"
#include "Assets.h"
#include "Scene.h"
#include "Enemy.h"

Player* makePlayer(float cameraHeight, int canFly) {
    Player* player = malloc(sizeof(Player));

    player->object = makeObject(CUBE_SHAPE, PLAIN_TEXTURE, 0.8, 0.8, 0.8);
    player->cameraHeight = cameraHeight;
    player->canFly = canFly;

    vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
    vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
    memcpy(player->cameraFront, cameraFront, sizeof(vec3));
    memcpy(player->cameraUp, cameraUp, sizeof(vec3));

    player->yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    player->pitch = 0.0f;

    player->duration = -1;

    player->jumpHeight = 0;
    player->dJumpHeight = 0;
    
    setPlayerPosition(player, 0, 0);

    return player;
}



void setPlayerPosition(Player* player, float x, float z) {
    vec3 camPos = { x, player->cameraHeight + player->jumpHeight, z };
    setPlayerCameraPosition(player, camPos);
}

void setPlayerCameraPosition(Player* player, vec3 pos) {
    memcpy(player->cameraPos, pos, sizeof(vec3));

    setObjectPosition(player->object, pos[0], pos[1] - player->cameraHeight + player->object->height / 2, pos[2]);
}


void updatePlayer(Player* player, float delta) {
    if (player->dJumpHeight != 0) {
        player->jumpHeight += player->dJumpHeight*PLAYER_JUMP_SHIFT*delta;
        if (player->jumpHeight < 0) {
            player->dJumpHeight = 0;
            player->jumpHeight = 0;
        }
        if (player->jumpHeight > player->cameraHeight * PLAYER_MAX_JUMP_RANGE) {
            player->dJumpHeight = -1;
        }

        setPlayerPosition(player, player->object->position[0], player->object->position[2]);
    }

    player->duration -= delta;
    if (player->duration < 0) return;

    vec3 oldPos; memcpy(oldPos, player->object->position, sizeof(vec3));
    updateObjectPosition(player->object, player->force, delta*PLAYER_SPEED);
    if (isObjectOutOfScene(player->object) || makeCollisionPlayerBoxes(player) || makeCollisionPlayerEnemies(player)) {
        memcpy(player->object->position, oldPos, sizeof(vec3));
    }

    setPlayerPosition(player, player->object->position[0], player->object->position[2]);
}

void setPlayerForce(Player* player, vec3 force, float duration) {
    memcpy(player->force, force, sizeof(vec3));
    player->duration = duration;
}

void makePlayerJump(Player* player) {
    if (player->dJumpHeight == 0) {
        player->dJumpHeight = 1;
        player->jumpHeight = 0;
    }
}

void movePlayerUp(Player* player) {
    vec3 tmp;
    memcpy(tmp, player->cameraFront, sizeof(vec3));
    
    if (!player->canFly) {
        tmp[1] = 0;
    }
    vec3_norm(tmp, tmp);

    setPlayerForce(player, tmp, MOVE_DURATION);
}

void movePlayerDown(Player* player) {
    vec3 tmp;
    memcpy(tmp, player->cameraFront, sizeof(vec3));
    
    if (!player->canFly) {
        tmp[1] = 0;
    }

    vec3_norm(tmp, tmp);
    vec3_scale(tmp, tmp, -1);

    setPlayerForce(player, tmp, MOVE_DURATION);
}

void movePlayerLeft(Player* player) {
    vec3 tmp;
    vec3_mul_cross(tmp, player->cameraFront, player->cameraUp);
    vec3_norm(tmp, tmp);
    vec3_scale(tmp, tmp, -1);

    setPlayerForce(player, tmp, MOVE_DURATION);
}

void movePlayerRight(Player* player) {
    vec3 tmp;
    vec3_mul_cross(tmp, player->cameraFront, player->cameraUp);
    vec3_norm(tmp, tmp);

    setPlayerForce(player, tmp, MOVE_DURATION);
}

void rotatePlayerSight(Player* player, float xoffset, float yoffset) {
    player->yaw += xoffset;
    player->pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (player->pitch > 89.0f)
        player->pitch = 89.0f;
    if (player->pitch < -89.0f)
        player->pitch = -89.0f;

    vec3 front;
    front[0] = cos(asDegree(player->yaw)) * cos(asDegree(player->pitch));
    front[1] = sin(asDegree(player->pitch));
    front[2] = sin(asDegree(player->yaw)) * cos(asDegree(player->pitch));
    vec3_norm(player->cameraFront, front);
}

void setPlayerPerspective(Player* player) {
    GLuint program = GLOBAL_PROGRAM;

    // Create transformations
    mat4x4 view;
    mat4x4 projection;
    mat4x4_identity(view); mat4x4_identity(projection);

    vec3 tmp;
    vec3_add(tmp, player->cameraPos, player->cameraFront);
    mat4x4_look_at(view, player->cameraPos, tmp, player->cameraUp);

    mat4x4_perspective(projection, asDegree(45), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.3f, 75.0f);
    // Get their uniform location
    GLint viewLoc = glGetUniformLocation(program, "view");
    GLint projLoc = glGetUniformLocation(program, "projection");
    // Pass them to the shaders
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
}

void playerShoot(Player* player) {
    addBulletToScene(player);
}


float angle = 0.0f;
int rotationEnd = 0;
float timeToEnd = 0;
void playerGameEndRotation(Player* player, Enemy* killer, float delta) {
    if (rotationEnd) {
        if (timeToEnd < 0) {
            rotationEnd = 0;
            endGameInScene();
            return;
        }
        timeToEnd -= delta;
        return;
    }

    float yoffset = -player->pitch * delta * PITCH_STABILIZATION;
    if (killer == NULL) { // no killer, game won, 360 of winner 
        rotatePlayerSight(player, PLAYER_END_ROTATION*delta, yoffset);
        angle += PLAYER_END_ROTATION;
        if (angle > 360.0f) {
            angle = 0.0f;
            rotationEnd = 1;
            timeToEnd = 1;
        }
    }
    else { // rotate to see your killer
        vec3 lookPos;
        vec3 beginFront, endFront;
        memcpy(lookPos, killer->head->position, sizeof(vec3));
        vec3_sub(lookPos, lookPos, player->cameraPos);
        memcpy(beginFront, player->cameraFront, sizeof(vec3));

        float r1 = vecIlo(beginFront[0], beginFront[2], lookPos[0], lookPos[2]);
        rotatePlayerSight(player, sign(r1)*PLAYER_END_ROTATION*delta, yoffset);
        
        memcpy(endFront, player->cameraFront, sizeof(vec3)); 
        float r2 = vecIlo(endFront[0],endFront[2], lookPos[0], lookPos[2]);

        if (r1*r2 < 0 ){
            rotationEnd = 1;
            timeToEnd = 2;
        }
    }
    
}