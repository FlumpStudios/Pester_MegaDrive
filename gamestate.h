#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "common.h"
#include "ui.h"
#include "enemies.h"
#include "player.h"
#include "background.h"
#include "game_update_observable.h"

#define STARTING_LEVEL 1
#define STARTING_LIVES 2



void ST_init(void);
void destructState(void);
void startGame(void);
void restartGame(void);
void endGame(void);
void increaseScore(u32 score);
void resetLevelTime(void);

bool isGamePlaying(void);
u8 getGameState(void);
u32 getLevelTime(void);
int getScore(void);
int getHighScore(void);

void addLife(void);
void removeLife(void);
u8 getLivesCount(void);

u8 getCurrentLevel(void);
void increaseCurrentLevel(void);

bool ST_is_game_paused(void);
void ST_set_is_game_paused(bool isPaused);
#endif