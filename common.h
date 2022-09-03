#ifndef COMMON_H
#define COMMON_H

#include <genesis.h>
#include "resources.h"
#include <memory.h>
#define DEACTIVATED_POSITION -100

// Static text
#define LABEL_SCORE "SCORE"
#define MSG_START "Press START to Begin!"
#define MSG_RESET  "Game over! Press START to Play Again."

// Shouldn't have done this :(
#define true TRUE
#define false FALSE

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

typedef struct 
{
	s16 left, right, top, bottom;
} Edges_t;


typedef struct
{
    Rectangle_t rect;
    Sprite *sprite;
} Entity_t;


typedef struct
{
	Entity_t;
	s16 speed;
    Vector2_t velocity;
} Actor_t;

// Common util functions
u16 generateRandomNum(int upper, s32 seed);
void *MEM_realloc(u16 new_size, void *d);

#endif