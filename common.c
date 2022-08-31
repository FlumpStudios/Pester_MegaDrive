#include "common.h"

u16 generateRandomNum(int upper, s32 seed)
{
	setRandomSeed(seed);
	return random() % upper;
}

// Super basic realloc, only realloc to larger
void *MEM_realloc(u16 new_size, void *d)
{
	void* response = MEM_alloc(new_size);
	memset(response,0, new_size);
	memcpy(response, d, new_size);
	MEM_free(d);
	return response;
}