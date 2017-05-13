#ifndef DEFINES_H
#define DEFINES_H

#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

#define SCENE_WIDTH 50.0f
#define SCENE_DEPTH 50.0f

#define BOARD_FALL -0.001f
#define WALL_HEIGHT 5.0f
#define WAVE_DESCRIPTION_HEIGHT 2.0f
#define BULLET_SPEED 30.0f
#define OBJECT_SHIFT 5.0f

#define MOVE_DURATION 0.05f
#define MOVE_BY_BULLET_DURATION 0.3f

#define MAX_BOXES 15
#define MAX_ENEMIES 100
#define MAX_BULLETS 10

#define PLAYER_JUMP_SHIFT 3.3f
#define PLAYER_SPEED 1.0f
#define PLAYER_MAX_JUMP_RANGE 0.5f
#define ENEMY_SPEED 0.85f
#define ENEMY_FLUCTUATION_SPEED 1.85f
#define ENEMY_FLUCTUATION_RANGE 0.15f

#define ENEMY_ABOVE 0.4f
#define ENEMY_HEAD_ABOVE 0.1f
#define ENEMY_CHASE_DURATION 0.2f
#define ENEMY_LIFE 3

#define HEAD_DMG 3
#define BODY_DMG 1

#define VF_WIDTH 0.01f
#define VF_HEIGHT 0.03f
#define VF_OFF 0.03f

#define PLAYER_END_ROTATION 66.6f
#define LAST_WAVE 5
#define NEXT_WAVE_WAIT_TIME 5.0f
#define PITCH_STABILIZATION 5.0f;

#define NO_STATUS 0
#define LOST_STATUS 1
#define WON_STATUS 2

#define MOUSE_SENSIVITY 0.03f

#endif // !DEFINES_H
