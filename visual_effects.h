#ifndef VISUAL_EFFECTS_H
#define VISUAL_EFFECTS_H
#include "common.h"

void VX_init(void);
void VX_spawnExposion(Rectangle_t position);
void VX_spawnExposionAtPosition(s16 x, s16 y);

void VX_spawnLargeExplosion(s16 x, s16 y, u32 time, u8 interval);
void VX_spawn_bullet_hit_effect(s16 x, s16 y);
#endif