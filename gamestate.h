#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "common.h"

void resetGameState(void);
void initiateGameState(void);
void resetGameTime(void);
void increaseScore(int score);
void setGamePlaying(bool isGamePlaying);
void setGameState(u8 gameState);
void setTimeMod(u8 timeMod);
void tickPlayTime(void);
void tickGameTime(void);
bool isGamePlaying(void);
u8 getGameState(void);
u8 getTimeMod(void);
u32 getPlayTime(void);
u32 getGameTime(void);
int getScore(void);
void destructState(void);
void resetScore(void);
#endif
