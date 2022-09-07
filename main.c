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
#include "game_script.h"
#include "ui.h"

#define GAME_NAME "PESTER!"

void gameScript(void)
{
	u32 game_time = getGameTime();
	if (game_time > 10)
	{
		// TODO: This is where we'll add the games script
	}
}

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
	VDP_setPalette(PAL1, tile.palette->data);
	VDP_setPalette(PAL2, paddle.palette->data);
	SYS_enableInts();

	// Set the text plane to Plane B so texts are drawn above the tiles
	VDP_setTextPlan(PLAN_B);

	VDP_drawText(LABEL_SCORE, 1, 1);
	VDP_drawText(LABEL_HIGH, 483, 1);

	drawCentredText(MSG_START);
	SPR_init(0, 0, 0);
	VDP_drawText(GAME_NAME, 16, 10);
	
	ST_init();
	UI_init();	
	SCR_init();
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
	{	runTickFunctions();

		SPR_update();
		VDP_waitVSync();
	}

	destructGame();

	return (0);
}
