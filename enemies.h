#ifndef ENEMIES_H
#define ENEMIES_H
#include "gamestate.h"
#include "common.h"

typedef struct
{
    Actor_t;
    bool is_enabled;
    u16 current_health;
    u16 worth;
} ENY_Actor;

typedef struct
{
    ENY_Actor;
} ENY_bird_t;

ENY_bird_t *createBird(bool setAsEnabled);
void ResetBird(ENY_bird_t *birdptr, bool enabled);
void destroyBird(ENY_bird_t* birdptr);
#endif