#include "collision_detection.h"
#include "player.h"
#include "common.h"
#include <genesis.h>
#include "resources.h"
#include <string.h>
#include "gamestate.h"

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
f16 bg_scroll_speed = 0;

u8 exposion_frame_ticker;
bool is_explosion_rendered = false;

// Score variables
char str_score[3] = "0";

// Enemy setup
Sprite *bird_enemy[MAX_BIRD_COUNT];
Sprite *explo_enemy;

Rectangle_t bird_Rect[MAX_BIRD_COUNT];
Vector2_t bird_velocity = {1, 2};
bool is_bird_enabled = false;
/*<--------------------------------------------------->*/

/*<--------------------------------------------------->*/
/* 					Helper functions 			       */
/*<--------------------------------------------------->*/
void showText(char s[])
{
	VDP_drawText(s, 20 - strlen(s) / 2, 15);
}

void updateScoreDisplay(void)
{
	sprintf(str_score, "%d", getScore());
	VDP_clearText(1, 2, 3);
	VDP_drawText(str_score, 1, 2);
}

void endGame(void)
{
	showText(msg_reset);
	setGamePlaying(false);
}

void initBird(void)
{
	u32 play_time = getPlayTime();
	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		bird_Rect[i].x = generateRandomNum(250, play_time + i);
		bird_Rect[i].y = generateRandomNum(200, play_time + i) * -1;
		bird_Rect[i].width = 32;
		bird_Rect[i].height = 32;
	};
}

void restartGame(void)
{
	setGamePlaying(true);
	resetPlayer();
	
	initBird();
	resetGameTime();
	VDP_clearTextArea(0, 10, 40, 10);
}

// The callback function that handles Joypad input
void handleInput(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		if (state & BUTTON_START)
		{
			if (getGameState() == MENU)
			{
				setGameState(GAME);
				createGameState();
			}

			if (!isGamePlaying())
			{
				restartGame();
			}
		}

		if (state & BUTTON_B)
		{
			if (isGamePlaying() && isShotOutOfBounds())
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

void renderExposion(Rectangle_t pos)
{
	is_explosion_rendered = true;
	SPR_setPosition(explo_enemy, pos.x, pos.y);
	increaseScore(10);
	updateScoreDisplay();
}

void exploFrameReset(void)
{
	if (is_explosion_rendered)
	{
		exposion_frame_ticker++;
		if (exposion_frame_ticker > 6)
		{
			SPR_setPosition(explo_enemy, DEACTIVATED_POSITION, DEACTIVATED_POSITION);
			exposion_frame_ticker = 0;
			is_explosion_rendered = false;
		}
	}
}

void renderBackground(void)
{
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 16, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 16, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 16, 0, CPU);
}

void createGameState(void)
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

void resetSprites(void)
{
	SPR_reset();
}

void updateBirdPosition(void)
{
	u32 game_time = getGameTime();
		
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
			bird_Rect[i].x = generateRandomNum(250, game_time + 1);
			bird_Rect[i].y = generateRandomNum(200, game_time + 1) * -1;
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
			bird_Rect[i].y = generateRandomNum(100, game_time + i) * -1;
		}

		SPR_setPosition(bird_enemy[i], bird_Rect[i].x, bird_Rect[i].y);
	};
}

void updateEnemyPositions(void)
{
	if (is_bird_enabled)
	{
		updateBirdPosition();
	}
}

void gameScript(void)
{
	u32 game_time = getGameTime();
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
	initiateGameState();
	/*<--------------------------------------------------->*/

	/* ----------------------------------------*/
	/* 				GAME LOOP!!! 			   */
	/* ----------------------------------------*/
	while (1)
	{
		tickPlayTime();
		if (isGamePlaying())
		{
			tickGameTime();
			gameScript();
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

		if (isGamePlaying())
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
