#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>
#include <memory.h>
#include "common.h"
#include "visual_effects.h"

// Initialiser
void PLY_init(void);

// Controls
void fireShot();
void moveLeft(bool seconAxisActive);
void moveRight(bool seconAxisActive);
void moveUp(bool seconAxisActive);
void moveDown(bool seconAxisActive);
void haltX(void);
void haltY(void);

// Actions
void PLY_set_boundary_checks_enabled(bool enabled);
void resetPlayer(void);
void destructPlayer(void);
void resetShot(void);
void runPlayerHit(void);
void enableShotMovement(void);
void disableShotMovement(void);
bool isShotOutOfBounds(void);
bool PLY_is_player_shot_enabled(void);
void PLY_disableShot(void);

// Getters
Rectangle_t getPlayerRect(void);
Rectangle_t getShotRect(void);
Rectangle_t getHitboxRect(void);
#endif
