#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "common.h"
#include "ui.h"
#include "enemies.h"
#include "player.h"
#include "background.h"
#include "game_update_observable.h"

#define STARTING_LEVEL 6
#define STARTING_LIVES 2

u32 GST_getChain(void);
void GST_increaseChain(u32 chain);
void GST_resetChain(void);

void GST_init(void);
void GST_destructState(void);
void GST_startGame(void);
void GST_restartGame(void);
void GST_endGame(void);
void GST_increaseScore(u32 score);
void GST_resetLevelTime(void);

bool GST_isGamePlaying(void);
u8 GST_getGameState(void);
u32 GST_getLevelTime(void);
int GST_getScore(void);
int GST_getHighScore(void);

void GST_addLife(void);
void GST_removeLife(void);
u8 GST_getLivesCount(void);

u8 GST_getCurrentLevel(void);
void GST_increaseCurrentLevel(void);

bool GST_is_game_paused(void);
void GST_set_is_game_paused(bool isPaused);
#endif