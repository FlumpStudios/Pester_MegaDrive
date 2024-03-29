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
void UI_updateChainDisplay(void);
void UI_display_warning();
void UI_clear_warning(void);
void UI_destruct(void);
void UI_CLEAR_TITLE_AS_TEXT(void);
void UI_DRAW_TITLE_AS_TEXT(void);
#endif
