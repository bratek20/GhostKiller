#include "Box.h"
#include <stdlib.h>
#include "Assets.h"
#include "Scene.h"
#include "Player.h"

Box* makeBox(float x, float z, float height, int moveable) {
    Box* box = malloc(sizeof(Box));

    box->object = makeObject(CUBE_SHAPE, METAL_BOX_TEXTURE, 2, height, 2);
    if (moveable) {
        box->object->texture = WOODEN_BOX_TEXTURE;
        setObjectColor(box->object, 0.7, 0.7, 0.7, 1);
    }

    box->moveable = moveable;
    setBoxPosition(box, x, z);

    box->duration = -1;

    return box;
}

Box* makeRandomBox(float height, int moveable) {
    Box* box = makeBox(0, 0, height, moveable);
    float x,z;
    do {
        x = rand() % ((int)SCENE_WIDTH / 3);
        z = rand() % ((int)SCENE_DEPTH / 3);
      
        int sign = rand() % 2;
        if (sign == 0) x *= -1;
        sign = rand() % 2;
        if (sign == 0) z *= -1;

        setBoxPosition(box, x, z);
    } while (isObjectOutOfScene(box->object) || makeCollisionBoxBoxes(box) || objectsCollide(box->object, GLOBAL_PLAYER->object));

    return box;
}

void updateBox(Box* box, float delta) {
    if (!box->moveable) return;

    box->duration -= delta;
    if (box->duration < 0) return;

    vec3 oldPos; memcpy(oldPos, box->object->position, sizeof(vec3));
    updateObjectPosition(box->object, box->force, delta);
    if (isObjectOutOfScene(box->object) || makeCollisionBoxBoxes(box) || makeCollisionBoxEnemies(box)) {
        memcpy(box->object->position, oldPos, sizeof(vec3));
    }
}

void setBoxPosition(Box* box, float x, float z) {
    setObjectPosition(box->object, x, box->object->height / 2, z);
}

void setBoxForce(Box* box, vec3 force, float duration) {
    memcpy(box->force, force, sizeof(vec3));
    box->duration = duration;
}