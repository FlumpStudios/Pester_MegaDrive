#ifndef ENEMIES_H
#define ENEMIES_H
#include "common.h"
#include "gamestate.h"

typedef struct
{
    bool is_enabled;
    u16 current_health;
    u16 worth;
    s16 speed;
    Vector2_t velocity;
    Rectangle_t rect;
    Sprite *sprite;
} enemyStateParent_t;

typedef struct
{
    enemyStateParent_t base;
} ENY_bird_t;

ENY_bird_t *createBird(bool setAsEnabled);
void ResetBird(ENY_bird_t *birdptr, bool enabled);
void destroyBird(ENY_bird_t* birdptr);

#endif