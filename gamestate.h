#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "common.h"

void initiateGameState(void);

void resetGameState(void);
void resetGameTime(void);
void increaseScore(int score);
void tickPlayTime(void);
void tickGameTime(void);
void destructState(void);
void resetScore(void);
bool isGamePlaying(void);

// Setters
void setGamePlaying(bool isGamePlaying);
void setGameState(u8 gameState);
void setTimeMod(u8 timeMod);

// Getters
u8 getGameState(void);
u8 getTimeMod(void);
u32 getPlayTime(void);
u32 getGameTime(void);
int getScore(void);
#endif
