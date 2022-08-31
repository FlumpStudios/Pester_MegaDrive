#ifndef GAME_H
#define GAME_H
#include "common.h"

typedef void (*tickFunction)(void);

void AddFunc(tickFunction f);
void RunStoredTickFunctions(void);

#endif