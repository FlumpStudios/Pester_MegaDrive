#include "game_script.h"
#include "enemies.h"
#include "game_update_observable.h"
#include "gamestate.h"
#include "controls.h"
#include "player.h"

#define MAX_LEVEL_DIDGETS 3
#define INTRO_LENGTH 300

static void endCurrentLevel(void)
{
    increaseCurrentLevel();
    resetLevelTime();
}

static void displayNewLevelText(void)
{
    char levelTextBuffer[6 + MAX_LEVEL_DIDGETS] = "Wave ";
    char levelNumberBuffer[MAX_LEVEL_DIDGETS];

    sprintf(levelNumberBuffer, "%d", getCurrentLevel());
    strcat(levelTextBuffer, levelNumberBuffer);
    UI_drawCentredText(levelTextBuffer);
}

static void startNewLevel(u32 level_time)
{
    if (level_time == 1)
    {
        UI_drawCentredText("Wave complete");
    }
    if (level_time == 100)
    {
        displayNewLevelText();
    }
    if (level_time == 200)
    {
        UI_clearCentredText();
    }
}

static void showWarning(u16 levelTime)
{
    if (levelTime % 50 == 0)
    {
        UI_display_warning();
    }
    else if (levelTime % 25 == 0)
    {
        UI_clear_warning();
    }
}

static void level_1(void)
{
    u32 level_time = getLevelTime();

    // ship move intro
    if (level_time == 1)
    {
        CTR_set_locked_controls(true);
        PLY_set_boundary_checks_enabled(false);
    }

    if (level_time > 75 && level_time < 150)
    {
        moveDown();
    }

    if (level_time == 150)
    {
        haltY();
        CTR_set_locked_controls(false);
        PLY_set_boundary_checks_enabled(true);
    }
    // Intro stops here

    if (level_time == 10)
    {
        UI_drawCentredText("Get Ready");
    }

    if (level_time == 100)
    {
        displayNewLevelText();
    }

    if (level_time == 300)
    {
        UI_clearCentredText();
    }

    if (level_time == 320)
    {
        ENY_spawnPopcorn(75, -40, 1, 1, 2);        
        ENY_spawnPopcorn(200, -40, 1, 1, 2);        
    }

    if (level_time == 350)
    {
        ENY_spawnPopcorn(75, -40, 1, 1, 2);        
        ENY_spawnPopcorn(200, -40, 1, 1, 2);
    }

    if (level_time == 380)
    {
        ENY_spawnPopcorn(75, -40, 1, 1, 2);        
        ENY_spawnPopcorn(200, -40, 1, 1, 2); 
    }

    if (level_time == 410)
    {
        ENY_spawnPopcorn(75, -40, 1, 1, 2);        
        ENY_spawnPopcorn(200, -40, 1, 1, 2); 
    }

    if (level_time == 540)
    {
        ENY_spawnPopcorn(150, -40, 0, 1, 2); 
    }

    if (level_time == 570)
    {
        ENY_spawnPopcorn(125, -40, 0, 1, 2);        
        ENY_spawnPopcorn(175, -40, 0, 1, 2); 
    }

    if (level_time == 600)
    {
        ENY_spawnPopcorn(150, -40, 0, 1, 2); 
    }

    if (level_time == 720)
    {
        ENY_spawnPopcorn(150, -40, 1, 1, 2); 
    }

    if (level_time == 740)
    {
        ENY_spawnPopcorn(175, -40, 1, 1, 2); 
        ENY_spawnPopcorn(125, -40, 1, 1, 2);
    }

    if (level_time == 760)
    {
        ENY_spawnPopcorn(200, -40, 1, 1, 2); 
        ENY_spawnPopcorn(100, -40, 1, 1, 2);
    }

    if (level_time == 780)
    {
        ENY_spawnPopcorn(225, -40, 1, 1, 2); 
        ENY_spawnPopcorn(75, -40, 1, 1, 2);
    }


    if (level_time == 1000)
    {
        ENY_spawnPopcorn(150, -40, -5, 1, 2); 
        ENY_spawnPopcorn(150, -40, 5, 1, 2);
    }

    if (level_time == 1030)
    {
        ENY_spawnPopcorn(150, -40, -5, 1, 2); 
        ENY_spawnPopcorn(150, -40, 5, 1, 2);
    }

    if (level_time == 1060)
    {
        ENY_spawnPopcorn(150, -40, -5, 1, 2); 
        ENY_spawnPopcorn(150, -40, 5, 1, 2);
    }


    if (level_time == 1220)
    {
        endCurrentLevel();
    }


    // if (level_time == 400)
    // {
    //     ENY_spawnGrabber(-50, -25, 3, 1);
    //     ENY_spawnGrabber(300, -25, -2, 1);
    // }

    // if (level_time == 500)
    // {
    //     ENY_spawnBird(150, -25, 1, 2);
    // }

    // if (level_time == 700)
    // {
    //     endCurrentLevel();
    //     PAL_fadeOutPalette(PAL3, 50, true);
    // }
}

static void level_2(void)
{
    u32 level_time = getLevelTime();
    if (level_time < INTRO_LENGTH)
    {
        startNewLevel(level_time);
    }

    if (level_time == 300)
    {
        BCK_draw_darkSky();
        PAL_fadeInPalette(PAL3, moon.palette->data, 50, true);

        ENY_spawnAstroid(100, -25, 0, 2);
        ENY_spawnAstroid(190, -35, 0, 1);
        ENY_spawnAstroid(150, -40, 0, 2);
    }

    if (level_time == 350)
    {
        ENY_spawnAstroid(55, -35, 0, 3);
        ENY_spawnAstroid(120, -30, 0, 2);
        ENY_spawnAstroid(200, -30, 0, 2);
    }
    if (level_time == 400)
    {
        ENY_spawnAstroid(90, -35, 0, 2);
        ENY_spawnAstroid(124, -60, 0, 3);
        ENY_spawnAstroid(210, -30, 0, 2);
    }

    if (level_time == 450)
    {
        ENY_spawnAstroid(40, -25, 0, 1);
        ENY_spawnAstroid(160, -200, 0, 3);
        ENY_spawnAstroid(220, -30, 0, 2);
    }

    if (level_time == 500)
    {
        ENY_spawnAstroid(100, -30, 0, 2);
        ENY_spawnAstroid(190, -45, 0, 1);
        ENY_spawnAstroid(150, -42, 0, 2);
        ENY_spawnGrabber(-50, -42, 1, 1);
        ENY_spawnGrabber(300, -20, -3, 1);
    }

    if (level_time == 550)
    {
        ENY_spawnAstroid(55, -26, 0, 3);
        ENY_spawnAstroid(120, -32, 0, 2);
        ENY_spawnAstroid(200, -31, 0, 3);
        ENY_spawnGrabber(-50, -35, 3, 1);
        ENY_spawnGrabber(300, -27, -2, 1);
    }
    if (level_time == 600)
    {
        ENY_spawnAstroid(90, -30, 0, 2);
        ENY_spawnAstroid(124, -60, 0, 3);
        ENY_spawnAstroid(210, -35, 0, 2);
    }

    if (level_time == 650)
    {
        ENY_spawnAstroid(40, -25, 0, 3);
        ENY_spawnAstroid(160, -250, 0, 3);
        ENY_spawnAstroid(220, -33, 0, 2);
        ENY_spawnGrabber(-50, -40, 3, 1);
        ENY_spawnGrabber(300, -31, -2, 1);
    }

    if (level_time == 900)
    {
        endCurrentLevel();
    }
}

static void level_3(void)
{
    u32 level_time = getLevelTime();
    if (level_time < INTRO_LENGTH)
    {
        startNewLevel(level_time);
    }

    if (level_time == 300)
    {
        ENY_spawnBouncer(80, -50, 5, 1, 150);
        ENY_spawnBouncer(200, -50, 5, 1, 150);
    }

    if (level_time == 450)
    {
        ENY_spawnFloater(100, -35, 2, 1);
        ENY_spawnFloater(180, -65, 1, 0);

        ENY_spawnGrabber(-600, -35, 3, 1);
        ENY_spawnGrabber(300, -25, -2, 1);
    }
}

void runscript(void)
{
    switch (getCurrentLevel())
    {
    case 0:
    case 1:
        level_1();
        break;
    case 2:
        level_2();
        break;
    case 3:
        level_3();
        break;
    }
}

void SCR_init(void)
{
    addTickFunc(runscript, true);
}