#ifndef ENEMY_CORE_H
#define ENEMY_CORE_H
#include "player.h"
#include "common.h"
#include "gamestate.h"

typedef struct
{
    Actor_t;
    u16 initial_health;
    u16 current_health;
    u16 worth;
    u16 lifeTime;
    u16 timeAlive;
    u16 timeOfLastHit;
    u8 variationId;    
} ENY_Actor_t;

ENY_Actor_t *ENY_new_enemy_actor(void);
void ENY_runSpawnSetup(ENY_Actor_t *a, s16 x, s16 y, s16 xSpeed, s16 ySpeed);
void ENY_runBulletSpawnSetup(Actor_t *a, s16 x, s16 y, s16 xSpeed, s16 ySpeed);
void ENY_reset(ENY_Actor_t *enyptr);
void ENY_reset_bullet(Actor_t *enyptr);
void ENY_kill(ENY_Actor_t *eny);
void ENY_handleHitByShot(ENY_Actor_t *eny);
void ENY_destroyEnemy(ENY_Actor_t *enyptr);
void ENY_destroyBullet(Actor_t *enyptr);
void ENY_checkShotCollision(ENY_Actor_t* enemy);

#endif