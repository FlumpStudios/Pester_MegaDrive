#include <genesis.h>
#include <string.h>
#include "resources.h"

#define true TRUE
#define false FALSE
#define BIRD_COUNT 3

#define Intro 0
#define Menu 1
#define Game 0


const int DEACTIVATED_POSITION = -100;


u8 currentGameState = Menu;

struct Rectangle
{
	f16 x, y;
	f16 height, width;
};

struct Vector
{
	f16 x, y;
};

struct Edges
{
	s16 left, right, top, bottom;
};

s32 playTime = 0;
s32 gameTime = 0;

f16 GenRandomNum(int upper)
{
	setRandomSeed(playTime);
	return random() % upper;
}

/*The sprites we'll need*/
Sprite *player;
Sprite *ball;
Sprite *birdEnemy[BIRD_COUNT];
Sprite *exploEnemy;
Sprite *hitBoxSprite;

u8 gameTimeMod = 1;
f16 bg_scroll_speed = 0;

//EXPLOSION
u8 frameTicker;
bool isRendered = false;

//Player setup
struct Rectangle player_Rect = {144, 160, 32, 32};
struct Rectangle hitBox_Rect = {144, 160, 2, 2};

struct Vector player_Velocity = {0, 0};
int playerSpeed = 3;
struct Edges edges = {0, 320, 0, 240};

struct Rectangle shot_Rect = {160, -14, 8, 8};
struct Vector shot_Velocity = {0, 0};
u8 shotSpeed = 6;

//Enemy setup
struct Rectangle bird_Rect[BIRD_COUNT];
struct Vector bird_Velocity = {1, 2};
bool isBirdEnabled = false;

bool game_on = false;

/*Score variables*/
s32 score = 0;
char label_score[6] = "SCORE\0";
char str_score[3] = "0";

/*Messages to be displayed*/
char msg_start[22] = "Press START to Begin!\0";
char msg_reset[37] = "Game over! Press START to Play Again.";

/*A small helper function to get the sign of an int*/
int sign(int x)
{
	return (x > 0) - (x < 0);
}

void positionPlayer()
{
	player_Rect.x += player_Velocity.x;
	player_Rect.y += player_Velocity.y;

	if (player_Rect.x < edges.left)
		player_Rect.x = edges.left;
	if (player_Rect.x + player_Rect.width > edges.right)
		player_Rect.x = edges.right - player_Rect.width;

	if (player_Rect.y < edges.top)
		player_Rect.y = edges.top;
	if (player_Rect.y + 50 > edges.bottom)
		player_Rect.y = edges.bottom - 50;

	hitBox_Rect.x = player_Rect.x + 12;
	hitBox_Rect.y = player_Rect.y + 16;

	SPR_setPosition(player, player_Rect.x, player_Rect.y);
	SPR_setPosition(hitBoxSprite, hitBox_Rect.x, hitBox_Rect.y);
}

void showText(char s[])
{
	VDP_drawText(s, 20 - strlen(s) / 2, 15);
}

void updateScoreDisplay()
{
	sprintf(str_score, "%d", score);
	VDP_clearText(1, 2, 3);
	VDP_drawText(str_score, 1, 2);
}

void endGame()
{
	showText(msg_reset);
	game_on = false;
}

void resetShot()
{
	shot_Rect.y = DEACTIVATED_POSITION;
}

void fireShot()
{
	shot_Rect.x = player_Rect.x + 12;
	shot_Rect.y = player_Rect.y;

	/*Clear the text from the screen*/
	updateScoreDisplay();	
}

bool collision(struct Rectangle obj, struct Rectangle obj2)
{
	//Check x intersects
	if (obj.x < obj2.x + obj2.width && obj.x + obj.width > obj2.x)
	{
		//Check y intersects
		if (obj.y < obj2.y + obj2.height && obj.y + obj.height >= obj2.y)
		{
			return true;
		}
	}
	return false;
}

/*Move the ball and handle collisions*/
void moveShot()
{
	shot_Rect.x += shot_Velocity.x;
	shot_Rect.y += shot_Velocity.y;

	/*Let the Sprite engine position the ball*/
	SPR_setPosition(ball, shot_Rect.x, shot_Rect.y);
}

void initBird()
{
	for (u8 i = 0; i < BIRD_COUNT; i++)
	{
		bird_Rect[i].x = GenRandomNum(250);
		bird_Rect[i].y = GenRandomNum(200) * -1;
		bird_Rect[i].width = 32;
		bird_Rect[i].height = 32;
	};
}

/*The callback function that handles Joypad input*/
void myJoyHandler(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		if (state & BUTTON_START)
		{
			if (currentGameState == Menu)
			{
				currentGameState = Game;
				ConstructGameState();
			}
			
			if (!game_on)
			{
				game_on = true;
				gameTime = 0;
				
				VDP_clearTextArea(0, 10, 40, 10);
			}
		}

		if (state & BUTTON_B)
		{
			if (game_on && shot_Rect.y <= 1)
			{
				fireShot();
			}
		}

		if (state & BUTTON_RIGHT)
		{
			player_Velocity.x = playerSpeed;
			SPR_setAnim(player, 2);
		}

		else if (state & BUTTON_LEFT)
		{
			player_Velocity.x = -playerSpeed;
			SPR_setAnim(player, 1);
		}
		else if ((changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT))
		{
			player_Velocity.x = 0;
			SPR_setAnim(player, 0);
		}

		if (state & BUTTON_UP)
			player_Velocity.y = -playerSpeed;

		else if (state & BUTTON_DOWN)
			player_Velocity.y = playerSpeed;

		else if ((changed & BUTTON_UP) | (changed & BUTTON_DOWN))
			player_Velocity.y = 0;
	}
}

void renderExposion(struct Rectangle pos)
{
	isRendered = true;
	SPR_setPosition(exploEnemy, pos.x, pos.y);
	score += 10;
}

void ExploFrameReset()
{
	if (isRendered)
	{
		frameTicker++;
		if (frameTicker > 6)
		{
			SPR_setPosition(exploEnemy, DEACTIVATED_POSITION, DEACTIVATED_POSITION);
			frameTicker = 0;
			isRendered = false;
		}
	}
}

void DrawBackground()
{
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 16, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 16, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 16, 0, CPU);
}

void ConstructGameState()
{
	DrawBackground();
	initBird();
	hitBox_Rect.x = player_Rect.x + 12;
	hitBox_Rect.y = player_Rect.y + 16;
	hitBoxSprite = SPR_addSprite(&hitBox, hitBox_Rect.x, hitBox_Rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

	for (u8 i = 0; i < BIRD_COUNT; i++)
	{
		birdEnemy[i] = SPR_addSprite(&bird, bird_Rect[i].x, bird_Rect[i].y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
	}

	exploEnemy = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 0, FALSE, FALSE));

	ball = SPR_addSprite(&imgball, shot_Rect.x, shot_Rect.y, TILE_ATTR(PAL2, 2, FALSE, FALSE));
	player = SPR_addSprite(&paddle, player_Rect.x, player_Rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
}

DestrcutGameState()
{
	SPR_reset();
	currentGameState = Menu;	
}

void updateBidPosition()
{
	for (u8 i = 0; i < BIRD_COUNT; i++)
	{
		if (bird_Rect[i].y > 0)
		{
			if (bird_Rect[i].x > player_Rect.x)
				bird_Rect[i].x -= bird_Velocity.x;
			else
				bird_Rect[i].x += bird_Velocity.x;
		}
		if (collision(bird_Rect[i], shot_Rect))
		{
			renderExposion(bird_Rect[i]);
			bird_Rect[i].x = GenRandomNum(250);
			bird_Rect[i].y = GenRandomNum(200) * -1;
			resetShot();
		}
		else if (collision(bird_Rect[i], hitBox_Rect))
		{
			endGame();
		}
		else
		{
			bird_Rect[i].y += bird_Velocity.y;
		}
		if (bird_Rect[i].y > 250)
			bird_Rect[i].y = GenRandomNum(100) * -1;
		SPR_setPosition(birdEnemy[i], bird_Rect[i].x, bird_Rect[i].y);
	};
}

void updateEnemyPositions()
{
	if (isBirdEnabled)
		updateBidPosition();
}

void Game_Script()
{
	if (gameTime > 10)
		isBirdEnabled = true;
}

int main()
{
	SYS_disableInts();

	JOY_init();
	JOY_setEventHandler(&myJoyHandler);

	VDP_setPalette(PAL2, paddle.palette->data);
	VDP_setPalette(PAL1, tile.palette->data);

	SYS_enableInts();

	/*Draw the texts*/
	VDP_setTextPlan(PLAN_B); /*Set the text plane to Plane B so texts are drawn above the tiles*/
	VDP_drawText(label_score, 1, 1);
	updateScoreDisplay();
	showText(msg_start);
	VDP_drawText("PESTER!", 16, 10);

	/*Add the sprites for the player and ball*/
	SPR_init(0, 0, 0);

	SPR_update();

	
	/**************
	 * GAME LOOP! *
	 **************/
	while (1)
	{
		playTime++;
		if (game_on)
		{
			gameTime += gameTimeMod;
			Game_Script();
		}

		bg_scroll_speed -= 2;

		if (bg_scroll_speed <= -250)
			bg_scroll_speed = 0;

		if (bg_scroll_speed <= -250)
			bg_scroll_speed = 0;

		if (shot_Rect.y > -10)
		{
			shot_Velocity.y = -shotSpeed;
		}
		else
		{
			shot_Velocity.y = 0;
		}
		VDP_setVerticalScroll(PLAN_A, bg_scroll_speed);

		if (game_on)
		{
			ExploFrameReset();
			moveShot();
			positionPlayer();
			updateEnemyPositions();
		}

		SPR_update();

		VDP_waitVSync();
	}
	return (0);
}
