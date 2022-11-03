#ifndef ui_h
#define ui_h
#include "common.h"

void UI_init(void);
void UI_drawHud(void);
void UI_drawCentredText(char s[]);
void UI_updateScoreDisplay(void);
void UI_updateHighScoreDisplay(void);
void UI_clearCentredText(void);
void UI_updateLivesText(void);
#endif
