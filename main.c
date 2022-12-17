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
#include "audio.h"
#define SKIP_INTRO TRUE

void destructGame(void)
{
	PLY_destructPlayer();
	GST_destructState();
	ENY_destruct_enemies();
}

void init_main(void)
{
	// SDK setups
	SYS_disableInts();
	JOY_init();	
	JOY_setEventHandler(&CTR_handleInput);

	SYS_enableInts();
	
	if (SKIP_INTRO != 1)
	{
		BCK_draw_intro_screen();
		PAL_fadeInPalette(PAL3, flump.palette->data, 150, false);
		waitMs(1000);
		PAL_fadeOutPalette(PAL3, 100, false);
		waitMs(200);
	}
	
	GST_setUpMainMenu();

	// module setups
	AUD_init();
	SPR_init(0, 0, 0);
	BCK_init();
	GST_init();
	SCR_init();
	VX_init();
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
