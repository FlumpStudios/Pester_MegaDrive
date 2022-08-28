#ifndef COMMON_H
#define COMMON_H

#include <genesis.h>
#include "resources.h"


#define true TRUE
#define false FALSE

#define INTRO 0
#define MENU 1
#define GAME 2

#define DEACTIVATED_POSITION -100

typedef struct 
{
	s16 x, y, height, width;
} Rectangle_t;

typedef struct Vector2_t
{
	f16 x, y;
} Vector2_t;

typedef struct 
{
	s16 left, right, top, bottom;
} Edges_t;

u16 generateRandomNum(int upper, s32 seed);
#endif