#include "common.h"

u16 generateRandomNum(int upper, s32 seed)
{
	setRandomSeed(seed);
	return random() % upper;
}