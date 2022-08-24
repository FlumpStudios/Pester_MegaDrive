#ifndef COMMON_H
#define COMMON_H
#include <genesis.h>
#include "resources.h"
#include <string.h>

#define true TRUE
#define false FALSE

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