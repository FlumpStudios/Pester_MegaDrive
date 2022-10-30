#ifndef ENEMY_CORE_H
#define ENEMY_CORE_H
#include "player.h"
#include "common.h"
#include "gamestate.h"

typedef struct
{
    Actor_t;
    bool is_enabled;
    u16 initial_health;
    u16 current_health;
    u16 worth;
    u16 lifeTime;
    u16 timeAlive;
} ENY_Actor;

void ENY_runSpawnSetup(ENY_Actor *a, s16 x, s16 y, s16 xSpeed, s16 ySpeed);
void ENY_reset(ENY_Actor *enyptr);
void ENY_kill(ENY_Actor *eny);
void ENY_handleHitByShot(ENY_Actor *eny);
void ENY_destroyEnemy(ENY_Actor *enyptr);
#endif