#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>
#include <memory.h>
#include "common.h"

// Initialiser
void PLY_init(void);

// Controls
void fireShot(void);
void moveLeft(void);
void moveRight(void);
void moveUp(void);
void moveDown(void);
void haltX(void);
void haltY(void);

// Actions
void PLY_set_boundary_checks_enabled(bool enabled);
void resetPlayer(void);
void destructPlayer(void);
void resetShot(void);
void killPlayer(void);
void enableShotMovement(void);
void disableShotMovement(void);
bool isShotOutOfBounds(void);

// Getters
Rectangle_t getPlayerRect(void);
Rectangle_t getShotRect(void);
Rectangle_t getHitboxRect(void);
#endif