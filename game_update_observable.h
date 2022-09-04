#ifndef GAME_H
#define GAME_H
#include "common.h"
typedef void (*tickFunction)(void);

void addTickFunc(tickFunction f, bool gameplay_only);
void runTickFunctions(void);
#endif