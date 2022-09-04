#include "ui.h"
#include "gamestate.h"

static char str_score[3] = "0";

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

void UI_init(void)
{
    updateScoreDisplay();   
}