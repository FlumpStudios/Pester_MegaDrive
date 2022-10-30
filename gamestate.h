#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "common.h"
#include "ui.h"
#include "enemies.h"
#include "player.h"
#include "background.h"
#include "game_update_observable.h"

void ST_init(void);
void destructState(void);

void startGame(void);
void restartGame(void);
void endGame(void);
void increaseScore(int score);
void setTimeMod(u8 timeMod);

bool isGamePlaying(void);
u8 getGameState(void);
u8 getTimeMod(void);
u32 getPlayTime(void);
u32 getGameTime(void);
int getScore(void);
int getHighScore(void);

u16 getCurrentLevel(void);
void increaseCurrentLevel(void);
#endif