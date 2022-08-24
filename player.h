#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include <genesis.h>
#include <memory.h>
#include "resources.h"

typedef struct
{
    Sprite *player_spr;
    Sprite *shot_spr;
    Sprite *hit_box_spr;
    struct Rectangle player_rect;
    struct Rectangle hitbox_rect;
    struct Vector2 velocity;
    struct Rectangle shot_rect;
    struct Vector2 shot_velocity;
    u8 speed;
    u8 shotSpeed;
} Player_t;


Player_t* GetInitialisedPlayer();
void ResetPlayer();
void DestructPlayer();
#endif