#include "common.h"

static void initialiseActor(Actor_t *e)
{
	e->rect.x = 0;
	e->rect.y = 0;
	e->rect.height = 0;
	e->rect.width = 0;

	e->speed = 0;
	
	e->velocity.x = 0;
	e->velocity.y = 0;

	e->is_enabled = false;
	e->spriteSlot1 = NULL;
	e->spriteSlot2 = NULL;
}

Actor_t* CMN_new_actor(void)
{
	Actor_t* result = MEM_alloc(sizeof(Actor_t));
	initialiseActor(result);
	return result;
}

u16 CMN_generateRandomNum(int upper, s32 seed)
{
	setRandomSeed(seed);
	return random() % upper;
}
// TODO: This is basically but potentially expensive, switch out when possible
u16 CMN_generateRandomNumWithRange(int lower, int upper, s32 seed)
{
	if (lower == upper)
	{
		return lower;
	}
	if (lower >= upper)
	{
		return 0;
	}

	setRandomSeed(seed);

	int response = random() % upper;

	if (response < lower)
	{
		return lower;
	}

	return response;
}

// Super basic realloc, only realloc to larger
void *CMN_mem_realloc(u16 new_size, void *d)
{
	void *response = MEM_alloc(new_size);
	memset(response, 0, new_size);
	memcpy(response, d, new_size);
	MEM_free(d);
	return response;
}