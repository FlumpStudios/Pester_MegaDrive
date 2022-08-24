#ifndef COMMON_H
#define COMMON_H

#include <genesis.h>

#define true TRUE
#define false FALSE

#define INTRO 0
#define MENU 1
#define GAME 2

struct Rectangle
{
	s16 x, y, height, width;
};

struct Vector2
{
	f16 x, y;
};

struct Edges
{
	s16 left, right, top, bottom;
};

u16 generateRandomNum(int upper, s32 seed);
#endif