#ifndef PLAYER_H
#define PLAYER_H
#include <genesis.h>
#include <memory.h>
#include "common.h"
#include "controls.h"
#include "visual_effects.h"

// Initialiser
void PLY_init(void);

// Controls
void PLY_fireShot(void);
void PLY_moveLeft(bool seconAxisActive);
void PLY_moveRight(bool seconAxisActive);
void PLY_moveUp(bool seconAxisActive);
void PLY_moveDown(bool seconAxisActive);
void PLY_haltX(void);
void PLY_haltY(void);

// Actions
void PLY_set_boundary_checks_enabled(bool enabled);
void PLY_resetPlayer(void);
void PLY_destructPlayer(void);
void PLY_resetShot(void);
void PLY_runPlayerHit(void);
void PLY_enableShotMovement(void);
void PLY_disableShotMovement(void);
bool PLY_isShotOutOfBounds(void);
bool PLY_is_player_shot_enabled(void);
void PLY_disableShot(void);
void PLY_disableSatelliteShot(void);
void PLY_enableSatelliteShot(void);
bool PLY_areSataliteShotsOutOfBounds(void);

// Getters
Rectangle_t PLY_getPlayerRect(void);
Rectangle_t PLY_getShotRect(void);
Rectangle_t PLY_getHitboxRect(void);
#endif
