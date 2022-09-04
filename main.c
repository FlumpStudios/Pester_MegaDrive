#include "collision_detection.h"
#include "player.h"
#include "controls.h"
#include "common.h"
#include <string.h>
#include "gamestate.h"
#include "enemies.h"
#include "game_update_observable.h"
#include "visual_effects.h"
#include "background.h"
#include "ui.h"

#define GAME_NAME "PESTER!"

// TODO : MOVE THE FOLLOWING FUNCTIONS OUT OF MAIN.C
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

void createGameState(void)
{
	BCK_init();
	PLY_init();
	VX_init();
	ENY_init();
}

void gameScript(void)
{
	u32 game_time = getGameTime();
	if (game_time > 10)
	{
		// TODO: This is where we'll add the games script
	}
}

// TODO ENDS HERE
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
	UI_init();	
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

		if (isGamePlaying())
		{
			tickGameTime();
			gameScript();						
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
