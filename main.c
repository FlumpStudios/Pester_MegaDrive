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
	VDP_setPalette(PAL2, bird.palette->data);
	VDP_setPalette(PAL3, introImage.palette->data);
	SYS_enableInts();
	VDP_setTextPlan(PLAN_B);	
	
	UI_drawHud();
	
	SPR_init(0, 0, 0);
	ST_init();
	UI_init();	
	SCR_init();
	
	BCK_init_title_screen();
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
	u8 intro_ticker = 0;
	while (1)
	{	
		// TODO: Move into menu file
		if(getGameState() ==  GAME_STATE_MENU)
		{
			intro_ticker++;
			if(intro_ticker == 1)
			{
				UI_drawCentredText(MSG_START);
			}
			if(intro_ticker > 30)
			{
				UI_clearCentredText();
			}
			if(intro_ticker > 45)
			{
				intro_ticker = 0;
			}
		}
		
				
		runTickFunctions();
		
		SPR_update();
		VDP_waitVSync();
	}

	destructGame();
	return (0);
}
