#include "common.h"
#include "collision_detection.h"

/* ---------------------------------------- */
/* 			Constants and defs				*/
/* ---------------------------------------- */
#define MAX_BIRD_COUNT 3

#define INTRO 0
#define MENU 1
#define GAME 2

const int DEACTIVATED_POSITION = -100;

// Static text
char label_score[6] = "SCORE";
char msg_start[22] = "Press START to Begin!";
char msg_reset[37] = "Game over! Press START to Play Again.";
/*<--------------------------------------------------->*/


/*<--------------------------------------------------->*/
/* 					 Declarations 		    		   */
/*<--------------------------------------------------->*/
u8 current_game_state = MENU;
s32 play_time, game_time = 0;

Sprite *player;
Sprite *playerShot;
Sprite *bird_enemy[MAX_BIRD_COUNT];
Sprite *explo_enemy;
Sprite *hit_box_sprite;

u8 game_time_mod = 1;
f16 bg_scroll_speed = 0;

u8 frame_ticker;
bool is_rendered = false;

// Player setup
struct Rectangle player_rect = {144, 160, 32, 32};
struct Rectangle hitbox_rect = {144, 160, 2, 2};

struct Vector2 player_velocity = {0, 0};
int player_speed = 3;
struct Edges edges = {0, 320, 0, 240};

struct Rectangle shot_rect = {160, -14, 8, 8};
struct Vector2 shot_velocity = {0, 0};
u8 shotSpeed = 6;

// Enemy setup
struct Rectangle bird_Rect[MAX_BIRD_COUNT];
struct Vector2 bird_velocity = {1, 2};
bool is_bird_enabled = false;
bool game_is_playing = false;

// Score variables
int score = 0;
char str_score[3] = "0";
/*<--------------------------------------------------->*/



/*<--------------------------------------------------->*/
/* 					Helper functions 			       */
/*<--------------------------------------------------->*/
void updatePlayerPosition()
{
	// Set positions
	player_rect.x += player_velocity.x;
	player_rect.y += player_velocity.y;

	// Boundary checks
	if (player_rect.x < edges.left)
	{
		player_rect.x = edges.left;
	}

	if (player_rect.x + player_rect.width > edges.right)
	{
		player_rect.x = edges.right - player_rect.width;
	}

	if (player_rect.y < edges.top)
	{
		player_rect.y = edges.top;
	}
	
	if (player_rect.y + 50 > edges.bottom)
	{
		player_rect.y = edges.bottom - 50;
	}

	// Position the hitbox
	hitbox_rect.x = player_rect.x + 12;
	hitbox_rect.y = player_rect.y + 16;

	// Set sprite position in SGDK
	SPR_setPosition(player, player_rect.x, player_rect.y);
	SPR_setPosition(hit_box_sprite, hitbox_rect.x, hitbox_rect.y);
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
	game_is_playing = false;
}

void resetShot()
{
	shot_rect.y = DEACTIVATED_POSITION;
}

void fireShot()
{
	shot_rect.x = player_rect.x + 12;
	shot_rect.y = player_rect.y;

	// Clear the text from the screen
	updateScoreDisplay();	
}

void moveShot()
{
	shot_rect.x += shot_velocity.x;
	shot_rect.y += shot_velocity.y;

	SPR_setPosition(playerShot, shot_rect.x, shot_rect.y);
}

void initBird()
{
	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		bird_Rect[i].x = generateRandomNum(250, play_time);
		bird_Rect[i].y = generateRandomNum(200, play_time) * -1;
		bird_Rect[i].width = 32;
		bird_Rect[i].height = 32;
	};
}

void resetPlayerPosition()
{
	player_rect.x = 144;
	player_rect.y = 160;

	hitbox_rect.x = 144;
	hitbox_rect.x = 160;
}

void restartGame()
{
	game_is_playing = true;
	initBird();
	resetPlayerPosition();
	
	game_time = 0;
	VDP_clearTextArea(0, 10, 40, 10);
}

// The callback function that handles Joypad input
void handleInput(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		if (state & BUTTON_START)
		{
			if (current_game_state == MENU)
			{
				current_game_state = GAME;
				createGameState();				
			}
			
			if (!game_is_playing)
			{
				restartGame();
			}
		}

		if (state & BUTTON_B)
		{
			if (game_is_playing && shot_rect.y <= 1)
			{
				fireShot();
			}
		}

		if (state & BUTTON_RIGHT)
		{
			player_velocity.x = player_speed;
			SPR_setAnim(player, 2);
		}
		else if (state & BUTTON_LEFT)
		{
			player_velocity.x = -player_speed;
			SPR_setAnim(player, 1);
		}
		else if ((changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT))
		{
			player_velocity.x = 0;
			SPR_setAnim(player, 0);
		}

		if (state & BUTTON_UP)
		{
			player_velocity.y = -player_speed;
		}

		else if (state & BUTTON_DOWN)
		{
			player_velocity.y = player_speed;
		}

		else if ((changed & BUTTON_UP) | (changed & BUTTON_DOWN))
		{
			player_velocity.y = 0;
		}
	}
}

void renderExposion(struct Rectangle pos)
{
	is_rendered = true;
	SPR_setPosition(explo_enemy, pos.x, pos.y);
	score += 10;
}

void exploFrameReset()
{
	if (is_rendered)
	{
		frame_ticker++;
		if (frame_ticker > 6)
		{
			SPR_setPosition(explo_enemy, DEACTIVATED_POSITION, DEACTIVATED_POSITION);
			frame_ticker = 0;
			is_rendered = false;
		}
	}
}

void renderBackground()
{
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 16, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 16, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 0, 0, CPU);
	VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 16, 0, CPU);
}

void createGameState()
{
	renderBackground();
	initBird();
	hitbox_rect.x = player_rect.x + 12;
	hitbox_rect.y = player_rect.y + 16;
	hit_box_sprite = SPR_addSprite(&hitBox, hitbox_rect.x, hitbox_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		bird_enemy[i] = SPR_addSprite(&bird, bird_Rect[i].x, bird_Rect[i].y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
	}

	explo_enemy = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 0, FALSE, FALSE));

	playerShot = SPR_addSprite(&imgball, shot_rect.x, shot_rect.y, TILE_ATTR(PAL2, 2, FALSE, FALSE));
	player = SPR_addSprite(&paddle, player_rect.x, player_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
}

void resetGameState()
{
	SPR_reset();
	current_game_state = MENU;	
}

void updateBirdPosition()
{
	for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
	{
		if (bird_Rect[i].y > 0)
		{
			if (bird_Rect[i].x > player_rect.x)
			{
				bird_Rect[i].x -= bird_velocity.x;
			}
			else
			{
				bird_Rect[i].x += bird_velocity.x;
			}
		}	
		if (checkRectangleCollision(&bird_Rect[i], &shot_rect))
		{
			renderExposion(bird_Rect[i]);
			bird_Rect[i].x = generateRandomNum(250, game_time);
			bird_Rect[i].y = generateRandomNum(200, game_time) * -1;
			resetShot();
		}
		else if (checkRectangleCollision(&bird_Rect[i], &hitbox_rect))
		{
			endGame();
		}
		else
		{
			bird_Rect[i].y += bird_velocity.y;
		}
		if (bird_Rect[i].y > 250)
		{
			bird_Rect[i].y = generateRandomNum(100, game_time) * -1;
		}

		SPR_setPosition(bird_enemy[i], bird_Rect[i].x, bird_Rect[i].y);
	};
}

void updateEnemyPositions()
{
	if (is_bird_enabled)
	{
		updateBirdPosition();
	}
}

void game_Script()
{
	if (game_time > 10)
	{
		is_bird_enabled = true;
	}
}
/*<--------------------------------------------------->*/


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
	VDP_setPalette(PAL1, tile.palette->data);	SYS_enableInts();
	
	// Set the text plane to Plane B so texts are drawn above the tiles
	VDP_setTextPlan(PLAN_B);

	VDP_drawText(label_score, 1, 1);
	updateScoreDisplay();
	showText(msg_start);
	VDP_drawText("PESTER!", 16, 10);
	SPR_init(0, 0, 0);
	SPR_update();
	/*<--------------------------------------------------->*/
	

	/* ----------------------------------------*/
	/* 				GAME LOOP!!! 			   */
	/* ----------------------------------------*/
	while (1)
	{
		play_time++;
		if (game_is_playing)
		{
			game_time += game_time_mod;
			game_Script();
		}

		bg_scroll_speed -= 2;

		if (bg_scroll_speed <= -250)
			bg_scroll_speed = 0;

		if (bg_scroll_speed <= -250)
			bg_scroll_speed = 0;

		if (shot_rect.y > -10)
		{
			shot_velocity.y = -shotSpeed;
		}
		else
		{
			shot_velocity.y = 0;
		}
		VDP_setVerticalScroll(PLAN_A, bg_scroll_speed);

		if (game_is_playing)
		{
			exploFrameReset();
			moveShot();
			updatePlayerPosition();
			updateEnemyPositions();
		}

		SPR_update();

		VDP_waitVSync();
	}
	return (0);
}
/*<--------------------------------------------------->*/

