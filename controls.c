#include "controls.h"
#include "gamestate.h"
#include "player.h"

static bool are_controls_locked = false;

void CTR_set_locked_controls(bool locked)
{
	are_controls_locked = locked;
}

void handleInput(u16 joy, u16 changed, u16 state)
{
	if (!are_controls_locked && joy == JOY_1)
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