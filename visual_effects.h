#ifndef VISUAL_EFFECTS_H
#define VISUAL_EFFECTS_H
#include "common.h"

void VX_init(void);
void VX_spawnExposion(Rectangle_t position);
void VX_spawnExposionAtPosition(u8 x, u8 y);
void VX_spawnLargeExplosion(u8 x, u8 y, u32 time, u8 interval);
void VX_spawn_bullet_hit_effect(u8 x, u8 y);
#endif