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
}