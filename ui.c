#include "ui.h"
#include "gamestate.h"

#define MAX_SCORE_LENGTH 5

static char score_str_bufer[MAX_SCORE_LENGTH] = "0";
static char high_str_buffer[MAX_SCORE_LENGTH] = "0";

void UI_clearCentredText(void)
{
	VDP_clearText(0, 15, 1000);
}

void UI_drawCentredText(char s[])
{
	UI_clearCentredText();
	VDP_drawText(s, 20 - strlen(s) / 2, 15);
}

void UI_drawHud(void)
{
	VDP_drawText(LABEL_SCORE, 1, 1);
	VDP_drawText(LABEL_HIGH, 483, 1);
}

void UI_updateScoreDisplay(void)
{
	sprintf(score_str_bufer, "%d", getScore());
	VDP_clearText(1, 2, MAX_SCORE_LENGTH);
	VDP_drawText(score_str_bufer, 1, 2);
}

void UI_updateHighScoreDisplay(void)
{
	u16 high = getHighScore();
	sprintf(high_str_buffer, "%d", high);
	VDP_clearText(1, 483, MAX_SCORE_LENGTH);
	VDP_drawText(high_str_buffer, 487 - strlen(high_str_buffer), 2);
}

void UI_init(void)
{
	UI_updateScoreDisplay();
	UI_updateHighScoreDisplay();
}