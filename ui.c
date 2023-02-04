#include "ui.h"
#include "gamestate.h"

#define MAX_SCORE_LENGTH 5
#define MAX_CHAIN_LENGTH 5

static char chain_str_bufer[MAX_CHAIN_LENGTH] = "0";
static char score_str_bufer[MAX_SCORE_LENGTH] = "0";
static char high_str_buffer[MAX_SCORE_LENGTH] = "0";

static Sprite* life_sprite = NULL;

void UI_clearCentredText(void)
{
	VDP_clearText(0, 15, 300);
}

void UI_DRAW_TITLE_AS_TEXT(void)
{
	VDP_drawText("PESTER 2", 16, 8);
}

void UI_CLEAR_TITLE_AS_TEXT(void)
{
	VDP_clearText(0, 8, 30);
}

	void UI_drawCentredText(char s[])
{
	UI_clearCentredText();
	VDP_drawText(s, 20 - strlen(s) / 2, 15);
}

void UI_display_warning(void)
{
	VDP_setPalette(PAL0, palette_red);	
	u8 xPos = 10;
	UI_clearCentredText();
	VDP_drawText("********************", xPos, 11);
	VDP_drawText("*//////////////////*", xPos, 12);
	VDP_drawText("*//              //*", xPos, 13);
	VDP_drawText("*//   WARNING!   //*", xPos, 14);
	VDP_drawText("*//              //*", xPos, 15);
	VDP_drawText("*//////////////////*", xPos, 16);
	VDP_drawText("********************", xPos, 17);
}

void UI_clear_warning(void)
{
	VDP_setPalette(PAL0, palette_grey);	
	
	for (u8 i = 11; i < 18; i++)
	{
		VDP_clearText(0, i, 300);
	}
}

void UI_updateLivesText(void)
{
	u8 lives = GST_getLivesCount();
	char buffer[2];
	sprintf(buffer, "X%u", lives);
	VDP_drawText(buffer, 6, 26);
}

void UI_drawHud(void)
{	 
	VDP_drawText(CHAIN_HIGH, 3, 4);	
	VDP_drawText(LABEL_SCORE, 3, 1);
	VDP_drawText(LABEL_HIGH, 481, 1);
	life_sprite = SPR_addSprite(&lifeSprite, 24, 203, TILE_ATTR(PAL2, 0, FALSE, FALSE));
}

void UI_updateScoreDisplay(void)
{
	sprintf(score_str_bufer, "%lu", GST_getScore());
	VDP_clearText(1, 2, MAX_SCORE_LENGTH);
	VDP_drawText(score_str_bufer, 3, 2);
}

void UI_updateChainDisplay(void)
{
	sprintf(chain_str_bufer, "%lu", GST_getChain());
	VDP_clearText(3, 5, MAX_CHAIN_LENGTH);
	VDP_drawText(chain_str_bufer, 3, 5);
}

void UI_updateHighScoreDisplay(void)
{
	u32 high = GST_getHighScore();
	sprintf(high_str_buffer, "%lu", high);
	VDP_clearText(1, 483, MAX_SCORE_LENGTH);
	VDP_drawText(high_str_buffer, 485 - strlen(high_str_buffer), 2);
}

void UI_init(void)
{
	UI_clearCentredText();
	UI_updateScoreDisplay();
	UI_updateHighScoreDisplay();
	UI_updateLivesText();
	UI_drawHud();
}

void UI_destruct(void)
{
	if(life_sprite != NULL)
	{
		VDP_clearPlan(PLAN_A, 0);
		SPR_releaseSprite(life_sprite);
		life_sprite = NULL;
	}
}