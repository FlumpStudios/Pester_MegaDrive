#include "collision_detection.h"
#include "player.h"
#include "common.h"
#include <string.h>
#include "gamestate.h"
#include "enemies.h"

/* ---------------------------------------- */
/* 			Constants and defs				*/
/* ---------------------------------------- */
#define MAX_BIRD_COUNT 3

// Static text
char label_score[6] = "SCORE";
char msg_start[22] = "Press START to Begin!";
char msg_reset[40] = "Game over! Press START to Play Again.";
/*<--------------------------------------------------->*/

/*<--------------------------------------------------->*/
/* 					 Declarations 		    		   */
/*<--------------------------------------------------->*/
f16 bg_scroll_speed = 0;
u8 exposion_frame_ticker;
bool is_explosion_rendered = false;

// Score variables
char str_score[3] = "0";
Sprite *explo_enemy;

// Enemy setup
ENY_bird_t* birdEnemies[MAX_BIRD_COUNT];

bool is_bird_enabled = false;
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
	showText(msg_reset);
	setGamePlaying(false);
}

void restartGame(void)
{
	setGamePlaying(true);
	resetPlayer();

	// Reset enemies
	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		ResetBird(birdEnemies[i], true);
	};

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
			if (getGameState() == MENU)
			{
				setGameState(GAME);
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

void renderExposion(Rectangle_t pos)
{
	is_explosion_rendered = true;
	SPR_setPosition(explo_enemy, pos.x, pos.y);
	increaseScore(10);
	updateScoreDisplay();
}

void exploFrameReset(void)
{
	if (is_explosion_rendered)
	{
		exposion_frame_ticker++;
		if (exposion_frame_ticker > 6)
		{
			SPR_setPosition(explo_enemy, DEACTIVATED_POSITION, DEACTIVATED_POSITION);
			exposion_frame_ticker = 0;
			is_explosion_rendered = false;
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
	initialisedPlayer();

	// Initiate bird enemies
	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		birdEnemies[i] = createBird(true);
	}

	explo_enemy = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 0, FALSE, FALSE));
}

void resetSprites(void)
{
	SPR_reset();	
}

void updateBirdPosition(void)
{	
	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		if (birdEnemies[i]->base.rect.y > 0)
		{
			if (birdEnemies[i]->base.rect.x > getPlayerRect().x)
			{
				birdEnemies[i]->base.rect.x -= birdEnemies[i]->base.velocity.y;
			}
			else
			{
				birdEnemies[i]->base.rect.x += birdEnemies[i]->base.velocity.x;
			}
		}
		if (checkRectangleCollision(birdEnemies[i]->base.rect, getShotRect()))
		{
			renderExposion(birdEnemies[i]->base.rect);
			ResetBird(birdEnemies[i], true);
			resetShot();
		}
		else if (checkRectangleCollision(birdEnemies[i]->base.rect, getHitboxRect()))
		{
			endGame();
		}
		else
		{
		 	birdEnemies[i]->base.rect.y += birdEnemies[i]->base.velocity.y;
		}
		if (birdEnemies[i]->base.rect.y > 250)
		{
			ResetBird(birdEnemies[i], true);
		}

		SPR_setPosition(birdEnemies[i]->base.sprite, birdEnemies[i]->base.rect.x, birdEnemies[i]->base.rect.y);
	};
}

void updateEnemyPositions(void)
{
	if (is_bird_enabled)
	{
		updateBirdPosition();
	}
}

void gameScript(void)
{
	u32 game_time = getGameTime();
	if (game_time > 10)
	{
		is_bird_enabled = true;
	}
}
/*<--------------------------------------------------->*/

void destructGame(void)
{
	destructPlayer();
	destructState();
	for (uint8_t i = 0; i < MAX_BIRD_COUNT; i++)
	{
		destroyBird(birdEnemies[i]);
	}
}

/* ---------------------------------------- */
/* 				Program entry 			    */
/* ---------------------------------------- */
int main()
{
	
	/* ----------------------------------------*/
	/* 				Initiate game 			   */
	/* ----------------------------------------*/
	SYS_disableInts();
	JOY_init();
	JOY_setEventHandler(&handleInput);
	VDP_setPalette(PAL2, paddle.palette->data);
	VDP_setPalette(PAL1, tile.palette->data);
	SYS_enableInts();

	// Set the text plane to Plane B so texts are drawn above the tiles
	VDP_setTextPlan(PLAN_B);

	VDP_drawText(label_score, 1, 1);
	showText(msg_start);
	SPR_init(0, 0, 0);
	VDP_drawText("PESTER!", 16, 10);

	initiateGameState();
	
	// Initiate game state must be run before updating score for the first time;
	updateScoreDisplay();
	
	/*<--------------------------------------------------->*/

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
			exploFrameReset();
			moveShot();
			updatePlayerPosition();
			updateEnemyPositions();
		}

		SPR_update();

		VDP_waitVSync();
	}
	
		destructGame();

	return (0);
}
/*<--------------------------------------------------->*/
