#include "collision_detection.h"
#include "player.h"
#include "common.h"
#include <string.h>
#include "gamestate.h"
#include "enemies.h"
#include "game_update_observable.h"
#include "visual_effects.h"

#define GAME_NAME "PESTER!"

/*<--------------------------------------------------->*/
/* 					 Declarations 		    		   */
/*<--------------------------------------------------->*/
f16 bg_scroll_speed = 0;

// Score variables
char str_score[3] = "0";

// Enemy setup

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
	showText(MSG_RESET);
	setGamePlaying(false);
}

void restartGame(void)
{
	setGamePlaying(true);
	resetPlayer();

	ENY_reset();

	resetScore();
	updateScoreDisplay();
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
			if (getGameState() == GAME_STATE_MENU)
			{
				setGameState(GAME_STATE_GAME);
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
	PLY_init();
	VX_init();
	ENY_init();
}


void gameScript(void)
{
	u32 game_time = getGameTime();
	if (game_time > 10)
	{
		// TODO: Add game script
	}
}
/*<--------------------------------------------------->*/

void destructGame(void)
{
	destructPlayer();
	destructState();
	ENY_destruct();
}

void init_main(void)
{
	SYS_disableInts();
	JOY_init();
	JOY_setEventHandler(&handleInput);
	VDP_setPalette(PAL2, paddle.palette->data);
	VDP_setPalette(PAL1, tile.palette->data);
	SYS_enableInts();

	// Set the text plane to Plane B so texts are drawn above the tiles
	VDP_setTextPlan(PLAN_B);

	VDP_drawText(LABEL_SCORE, 1, 1);
	showText(MSG_START);
	SPR_init(0, 0, 0);
	VDP_drawText(GAME_NAME, 16, 10);
	initiateGameState();

	// Initiate game state must be run before updating score for the first time;
	updateScoreDisplay();
}

/* ---------------------------------------- */
/* 				Program entry 			    */
/* ---------------------------------------- */
int main()
{
	init_main();

	/* ----------------------------------------*/
	/* 				GAME LOOP!!! 			   */
	/* ----------------------------------------*/
	while (1)
	{
		tickPlayTime();

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
			tickGameTime();
			gameScript();
			moveShot();
			updatePlayerPosition();			
		}

		// Runs any subscribed void function pointers
		runTickFunctions();
		SPR_update();
		VDP_waitVSync();
	}

	destructGame();

	return (0);
}
/*<--------------------------------------------------->*/
