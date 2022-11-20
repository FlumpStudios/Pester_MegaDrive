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

#define SKIP_INTRO TRUE

void destructGame(void)
{
	PLY_destructPlayer();
	GST_destructState();
	ENY_destruct();
}

void init_main(void)
{
	// SDK setups
	SYS_disableInts();
	JOY_init();
	JOY_setEventHandler(&CTR_handleInput);
	VDP_setPalette(PAL0, palette_grey);
	VDP_setPalette(PAL1, imgexplo.palette->data);
	VDP_setPalette(PAL2, bird.palette->data);
	VDP_setPalette(PAL3, palette_black);
	VDP_setTextPlan(PLAN_A);
	SYS_enableInts();

	// Initiate stuff
	if (SKIP_INTRO != 1)
	{
		BCK_draw_intro_screen();
		PAL_fadeInPalette(PAL3, flump.palette->data, 150, false);
		waitMs(1000);
		PAL_fadeOutPalette(PAL3, 100, false);
		waitMs(200);
	}

	BCK_draw_title_screen();
	PAL_fadeInPalette(PAL3, introImage.palette->data, 100, false);
	CTR_set_locked_controls(false);

	// module setups
	SPR_init(0, 0, 0);
	BCK_init();
	GST_init();
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
	u8 intro_ticker = 0;
	while (1)
	{
		// TODO: Move into menu file
		if (GST_getGameState() == GAME_STATE_MENU)
		{
			intro_ticker++;
			if (intro_ticker == 1)
			{
				UI_drawCentredText(MSG_START);
			}
			if (intro_ticker > 30)
			{
				UI_clearCentredText();
			}
			if (intro_ticker > 45)
			{
				intro_ticker = 0;
			}
		}

		if (!GST_is_game_paused())
		{
			runTickFunctions();
		}

		SPR_update();
		VDP_waitVSync();
	}

	destructGame();
	return (0);
}
