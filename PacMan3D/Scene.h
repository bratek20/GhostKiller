#ifndef SCENE_H
#define SCENE_H

typedef struct OBJECT Object;
typedef struct PLAYER Player;
typedef struct VIEW_FINDER ViewFinder;
typedef struct BOX Box;
typedef struct BULLET Bullet;
typedef struct ENEMY Enemy;

Player* GLOBAL_PLAYER;
ViewFinder* GLOBAL_VIEW_FINDER;

void initScene();
void clearScene();

void startGameInScene();
void startGameEndInScene(Enemy* killer);
void endGameInScene();
int isGameBeforeEndInScene();
int isGameEndedInScene();
int getLastSceneStatus();

void updateScene(float delta);
void drawScene();

void addBoxToScene(float x, float z, float height, int moveable);
void addRandomBoxToScene(float height, int moveable);
void addEnemyToScene();
int findDeadEnemy();
int countAliveEnemies();

void addBulletToScene(Player* shooter);
int findNotFlyingBullet();

int isObjectOutOfScene(Object* object);

int makeCollisionPlayerBoxes(Player* player);//returns 1 if player is colliding with Box
int makeCollisionPlayerEnemies(Player* player);

int makeCollisionBoxBoxes(Box* box);
int makeCollisionBoxEnemies(Box* box);

int makeCollisionBulletBoxes(Bullet* bullet);
int makeCollisionBulletEnemies(Bullet* bullet);

int makeCollisionEnemyBoxes(Enemy* enemy);
int makeCollisionEnemyEnemies(Enemy* enemy);

#endif