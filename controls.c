#include "controls.h"
#include "gamestate.h"
#include "player.h"

static bool are_controls_locked = true;

void CTR_set_locked_controls(bool locked)
{
	PLY_haltX();
	PLY_haltY();
	are_controls_locked = locked;
}

void CTR_handleInput(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		bool isPaused = GST_is_game_paused();
		if ((state & BUTTON_START) && GST_getGameState() == GAME_STATE_GAME)
		{
			if (isPaused)
			{
				GST_set_is_game_paused(false);
				UI_clearCentredText();
				PLY_haltY();
				PLY_haltX();
			}
			else
			{
				GST_set_is_game_paused(true);
				UI_drawCentredText("Game paused");
			}
		}

		if (!are_controls_locked && !isPaused)
		{
			if (state & BUTTON_START)
			{
				if (GST_getGameState() == GAME_STATE_MENU)
				{
					GST_startGame();
				}

				if (!GST_isGamePlaying())
				{
					GST_restartGame();
				}
			}

			if (state & BUTTON_B)
			{
				if (GST_getGameState() == GAME_STATE_MENU)
				{
					GST_startGame();
				}
				
				if (GST_isGamePlaying() && PLY_isShotOutOfBounds())
				{
					PLY_fireShot();
				}
			}

			if (state & BUTTON_RIGHT)
			{
				bool secondAxis = ((state & BUTTON_UP) || (state & BUTTON_DOWN));
				PLY_moveRight(secondAxis);
			}
			else if (state & BUTTON_LEFT)
			{
				bool secondAxis = ((state & BUTTON_UP) || (state & BUTTON_DOWN));
				PLY_moveLeft(secondAxis);
			}
			else
			{
				PLY_haltX();
			}

			if (state & BUTTON_UP)
			{
				bool secondAxis = ((state & BUTTON_LEFT) || (state & BUTTON_RIGHT));
				PLY_moveUp(secondAxis);
			}

			else if (state & BUTTON_DOWN)
			{
				bool secondAxis = ((state & BUTTON_LEFT) || (state & BUTTON_RIGHT));
				PLY_moveDown(secondAxis);
			}

			else
			{
				PLY_haltY();
			}
		}
	}
}