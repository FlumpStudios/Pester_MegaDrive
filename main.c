#include "collision_detection.h"
#include "player.h"
#include "common.h"
#include <genesis.h>
#include "resources.h"
#include <string.h>

/* ---------------------------------------- */
/* 			Constants and defs				*/
/* ---------------------------------------- */
#define MAX_BIRD_COUNT 3

// Static text
char label_score[6] = "SCORE";
char msg_start[22] = "Press START to Begin!";
char msg_reset[37] = "Game over! Press START to Play Again.";
/*<--------------------------------------------------->*/

/*<--------------------------------------------------->*/
/* 					 Declarations 		    		   */
/*<--------------------------------------------------->*/
Player_t *playerOne = NULL;

u8 current_game_state = MENU;
s32 play_time, game_time = 0;

Sprite *bird_enemy[MAX_BIRD_COUNT];
Sprite *explo_enemy;

u8 game_time_mod = 1;
f16 bg_scroll_speed = 0;

u8 frame_ticker;
bool is_rendered = false;

// Enemy setup
struct Rectangle bird_Rect[MAX_BIRD_COUNT];
struct Vector2 bird_velocity = {1, 2};
bool is_bird_enabled = false;
bool game_is_playing = false;

// Score variables
int score = 0;
char str_score[3] = "0";
/*<--------------------------------------------------->*/

/*<--------------------------------------------------->*/
/* 					Helper functions 			       */
/*<--------------------------------------------------->*/
void showText(char s[])
{
	VDP_drawText(s, 20 - strlen(s) / 2, 15);
}

void updateScoreDisplay()
{
	sprintf(str_score, "%d", score);
	VDP_clearText(1, 2, 3);
	VDP_drawText(str_score, 1, 2);
}

void endGame()
{
	showText(msg_reset);
	game_is_playing = false;
}

void initBird()
{
	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		bird_Rect[i].x = generateRandomNum(250, play_time);
		bird_Rect[i].y = generateRandomNum(200, play_time) * -1;
		bird_Rect[i].width = 32;
		bird_Rect[i].height = 32;
	};
}

void restartGame()
{
	game_is_playing = true;

	resetPlayer();
	initBird();

	game_time = 0;
	VDP_clearTextArea(0, 10, 40, 10);
}

// The callback function that handles Joypad input
void handleInput(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		if (state & BUTTON_START)
		{
			if (current_game_state == MENU)
			{
				current_game_state = GAME;
				createGameState();
			}

			if (!game_is_playing)
			{
				restartGame();
			}
		}

		if (state & BUTTON_B)
		{
			if (game_is_playing && isShotOutOfBounds())
			{
				fireShot();
			}
		}

		if (state & BUTTON_RIGHT)
		{
			moveRight();
		}
		else if (state & BUTTON_LEFT)
		{
			moveLeft();
		}
		else if ((changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT))
		{
			haltX();
		}

		if (state & BUTTON_UP)
		{
			moveUp();
		}

		else if (state & BUTTON_DOWN)
		{
			moveDown();
		}

		else if ((changed & BUTTON_UP) | (changed & BUTTON_DOWN))
		{
			haltY();
		}
	}
}

void renderExposion(struct Rectangle pos)
{
	is_rendered = true;
	SPR_setPosition(explo_enemy, pos.x, pos.y);
	score += 10;
	updateScoreDisplay();
}

void exploFrameReset()
{
	if (is_rendered)
	{
		frame_ticker++;
		if (frame_ticker > 6)
		{
			SPR_setPosition(explo_enemy, DEACTIVATED_POSITION, DEACTIVATED_POSITION);
			frame_ticker = 0;
			is_rendered = false;
		}
	}
}

void renderBackground()
{
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 16, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 16, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 16, 0, CPU);
}

void createGameState()
{
	renderBackground();
	initBird();
	initialisedPlayer();

	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		bird_enemy[i] = SPR_addSprite(&bird, bird_Rect[i].x, bird_Rect[i].y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
	}

	explo_enemy = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 0, FALSE, FALSE));
}

void resetGameState()
{
	SPR_reset();
	current_game_state = MENU;
}

void updateBirdPosition()
{
	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		if (bird_Rect[i].y > 0)
		{
			if (bird_Rect[i].x > getPlayerRect().x)
			{
				bird_Rect[i].x -= bird_velocity.x;
			}
			else
			{
				bird_Rect[i].x += bird_velocity.x;
			}
		}
		if (checkRectangleCollision(bird_Rect[i], getShotRect()))
		{
			renderExposion(bird_Rect[i]);
			bird_Rect[i].x = generateRandomNum(250, game_time);
			bird_Rect[i].y = generateRandomNum(200, game_time) * -1;
			resetShot();
		}
		else if (checkRectangleCollision(bird_Rect[i], getHitboxRect()))
		{
			endGame();
		}
		else
		{
			bird_Rect[i].y += bird_velocity.y;
		}
		if (bird_Rect[i].y > 250)
		{
			bird_Rect[i].y = generateRandomNum(100, game_time) * -1;
		}

		SPR_setPosition(bird_enemy[i], bird_Rect[i].x, bird_Rect[i].y);
	};
}

void updateEnemyPositions()
{
	if (is_bird_enabled)
	{
		updateBirdPosition();
	}
}

void game_Script()
{
	if (game_time > 10)
	{
		is_bird_enabled = true;
	}
}
/*<--------------------------------------------------->*/

/* ---------------------------------------- */
/* 				Program entry 			    */
/* ---------------------------------------- */
int main()
{
	/* ----------------------------------------*/
	/* 				Initiate game 			   */
	/* ----------------------------------------*/
	SYS_disableInts();
	JOY_init();
	JOY_setEventHandler(&handleInput);
	VDP_setPalette(PAL2, paddle.palette->data);
	VDP_setPalette(PAL1, tile.palette->data);
	SYS_enableInts();

	// Set the text plane to Plane B so texts are drawn above the tiles
	VDP_setTextPlan(PLAN_B);

	VDP_drawText(label_score, 1, 1);
	updateScoreDisplay();
	showText(msg_start);
	VDP_drawText("PESTER!", 16, 10);
	SPR_init(0, 0, 0);
	SPR_update();

	/*<--------------------------------------------------->*/

	/* ----------------------------------------*/
	/* 				GAME LOOP!!! 			   */
	/* ----------------------------------------*/
	while (1)
	{
		play_time++;
		if (game_is_playing)
		{
			game_time += game_time_mod;
			game_Script();
		}

		bg_scroll_speed -= 2;

		if (bg_scroll_speed <= -250)
			bg_scroll_speed = 0;

		if (bg_scroll_speed <= -250)
			bg_scroll_speed = 0;

		if (isShotOutOfBounds())
		{
			disableShotMovement();
		}
		else
		{
			enableShotMovement();
		}

		VDP_setVerticalScroll(PLAN_A, bg_scroll_speed);

		if (game_is_playing)
		{
			exploFrameReset();
			moveShot();
			updatePlayerPosition();
			updateEnemyPositions();
		}

		SPR_update();

		VDP_waitVSync();
	}
	return (0);
}
/*<--------------------------------------------------->*/
