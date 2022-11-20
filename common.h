#ifndef COMMON_H
#define COMMON_H

#include <genesis.h>
#include "resources.h"
#include <memory.h>

#define true TRUE
#define false FALSE

#define DEACTIVATED_POSITION 300

// Static text
#define LABEL_SCORE "SCORE"
#define LABEL_HIGH "HIGH"

#define MSG_START "Press START to Begin!"
#define MSG_RESET "Game over! Press START to Play Again."

// Game state
#define GAME_STATE_INTRO 0
#define GAME_STATE_MENU 1
#define GAME_STATE_GAME 2

// Shared structs
typedef struct
{
	s16 x, y, height, width;
} Rectangle_t;

typedef struct Vector2_t
{
	f16 x, y;
} Vector2_t;

typedef struct Range_t
{
	f16 min, max;
} Range_t;

typedef struct
{
	s16 left, right, top, bottom;
} Edges_t;

typedef struct
{
	Rectangle_t rect;
} Entity_t;

typedef struct
{
	Entity_t;
	s16 speed;
	Vector2_t velocity;
	bool is_enabled;    
	Sprite *spriteSlot1;
	Sprite *spriteSlot2;
} Actor_t;

// Common util functions
Actor_t* CMN_new_actor(void);

u16 CMN_generateRandomNum(int upper, s32 seed);
u16 CMN_generateRandomNumWithRange(int lower, int upper, s32 seed);
void *CMN_mem_realloc(u16 new_size, void *d);
#endif