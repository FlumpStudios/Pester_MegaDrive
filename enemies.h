#ifndef ENEMIES_H
#define ENEMIES_H
#include "collision_detection.h"
#include "player.h"
#include "common.h"
#include "gamestate.h"
#include "game_update_observable.h"
#include "visual_effects.h"

// Initiate all enemies
void ENY_init(void);

// reset all enemies
void ENY_reset(void);

// destruct all enemies
void ENY_destruct(void);
#endif