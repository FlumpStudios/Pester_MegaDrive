#include "controls.h"
#include "gamestate.h"
#include "player.h"

static bool are_controls_locked = true;

void CTR_set_locked_controls(bool locked)
{
	haltX();
	haltY();
	are_controls_locked = locked;
}

void handleInput(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		bool isPaused = ST_is_game_paused();
		if ((state & BUTTON_START) && getGameState() == GAME_STATE_GAME)
		{
			if (isPaused)
			{
				ST_set_is_game_paused(false);
				UI_clearCentredText();
				haltY();
				haltX();
			}
			else
			{
				ST_set_is_game_paused(true);
				UI_drawCentredText("Game paused");
			}
		}

		if (!are_controls_locked && !isPaused)
		{
			if (state & BUTTON_START)
			{
				if (getGameState() == GAME_STATE_MENU)
				{
					startGame();
				}

				if (!isGamePlaying())
				{
					restartGame();
				}
			}

			if (state & BUTTON_B)
			{
				if (getGameState() == GAME_STATE_MENU)
				{
					startGame();
				}
				
				if (isGamePlaying() && isShotOutOfBounds())
				{
					fireShot();
				}
			}

			if (state & BUTTON_RIGHT)
			{
				bool secondAxis = ((state & BUTTON_UP) || (state & BUTTON_DOWN));
				moveRight(secondAxis);
			}
			else if (state & BUTTON_LEFT)
			{
				bool secondAxis = ((state & BUTTON_UP) || (state & BUTTON_DOWN));
				moveLeft(secondAxis);
			}
			else
			{
				haltX();
			}

			if (state & BUTTON_UP)
			{
				bool secondAxis = ((state & BUTTON_LEFT) || (state & BUTTON_RIGHT));
				moveUp(secondAxis);
			}

			else if (state & BUTTON_DOWN)
			{
				bool secondAxis = ((state & BUTTON_LEFT) || (state & BUTTON_RIGHT));
				moveDown(secondAxis);
			}

			else
			{
				haltY();
			}
		}
	}
}