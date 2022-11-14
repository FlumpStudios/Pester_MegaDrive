#ifndef ENEMIES_H
#define ENEMIES_H
#include "collision_detection.h"
#include "visual_effects.h"
#include "enemy_core.h"
#include "game_update_observable.h"

// Variation defines
#define VARIATION_POPCORN_RIGHT 0
#define VARIATION_POPCORN_LEFT 1
#define VARIATION_POPCORN_CENTRE 2
#define VARIATION_SNAKE_CENTRE 3
#define VARIATION_SNAKE_RIGHT 4
#define VARIATION_SNAKE_LEFT 5

#define VARIATION_FLOATER_WITH_FORKED_BULLETS 0
#define VARIATION_FLOATER_WITH_SIDE_BULLETS 1

// Initiate all enemies
void ENY_init(void);

// reset all enemies
void ENY_resetAllEnemies(void);
void ENY_resetAllBullets(void);

// destruct all enemies
void ENY_destruct(void);

// Spawn enemies
void ENY_spawnGrabber(s16 x, s16 y, s16 xSpeed, s16 ySpeed);
void ENY_spawnBird(s16 x, s16 y, s16 xSpeed, s16 ySpeed);
void ENY_spawnAstroid(s16 x, s16 y, s16 xSpeed, s16 ySpeed);
void ENY_spawnBouncer(s16 x, s16 y, s16 xSpeed, s16 ySpeed, u16 lifeTime);
void ENY_spawnFloater(s16 x, s16 y,s16 ySpeed, u8 variation);
void ENY_spawnRocketBullet(s16 x, s16 y, s16 ySpeed);
void ENY_spawnPopcorn(s16 x, s16 y, s16 xSpeed,s16 ySpeed, u8 variation);

// Bullet patterns
void ENY_spawncircleBullets_forkedpattern(s16 x, s16 y);
void ENY_spawncircleBullets_squarepattern(s16 x, s16 y);
void ENY_spawncircleBullet(s16 x, s16 y, s16 xSpeed, s16 ySpeed);
void ENY_spawncircleBullets_sidepattern(s16 x, s16 y,  s8 speed);
#endif