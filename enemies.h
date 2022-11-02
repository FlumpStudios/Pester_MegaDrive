#ifndef ENEMIES_H
#define ENEMIES_H
#include "collision_detection.h"
#include "visual_effects.h"
#include "enemy_core.h"
#include "game_update_observable.h"

// Initiate all enemies
void ENY_init(void);

// reset all enemies
void ENY_resetAllEnemies(void);

// destruct all enemies
void ENY_destruct(void);

void ENY_spawnGrabber(s16 x, s16 y, s16 xSpeed, s16 ySpeed);
void ENY_spawnBird(s16 x, s16 y, s16 xSpeed,s16 ySpeed);
void ENY_spawnAstroid(s16 x, s16 y, s16 xSpeed,s16 ySpeed);
#endif