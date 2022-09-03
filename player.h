#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include <genesis.h>
#include <memory.h>
#include "resources.h"

void PLY_init(void);
void updatePlayerPosition(void);
void resetPlayer(void);
void destructPlayer(void);
void resetShot(void);
void moveShot(void);
void fireShot(void);
void moveLeft(void);
void moveRight(void);
void moveUp(void);
void moveDown(void);
void haltX(void);
void haltY(void);
void killPlayer(void);
void enableShotMovement(void);
void disableShotMovement(void);
bool isShotOutOfBounds(void);
Rectangle_t getPlayerRect(void);
Rectangle_t getShotRect(void);
Rectangle_t getHitboxRect(void);
#endif