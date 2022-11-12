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

static bool introHasRun = false;

static void runGameStartIntro(u16 level_time)
{
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
        introHasRun = true;
    }
}

static void startNewLevel(u32 level_time)
{
    if (!introHasRun)
    {
        runGameStartIntro(level_time);
    }
    else
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
    if (level_time < INTRO_LENGTH)
    {
        runGameStartIntro(level_time);
    }

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

    if (level_time == 1250)
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
        ENY_spawnFloater(100, -35, 1, 1);
        ENY_spawnFloater(200, -75, 1, 1);
        ENY_spawnFloater(100, -105, 1, 1);
    }
    if (level_time == 550)
    {
        ENY_spawnPopcorn(150, -40, 1, 1, 2);
    }

    if (level_time == 575)
    {
        ENY_spawnPopcorn(150, -40, 1, 1, 2);
    }
    if (level_time == 600)
    {
        ENY_spawnPopcorn(150, -40, 1, 1, 2);
    }
    if (level_time == 625)
    {
        ENY_spawnPopcorn(150, -40, 1, 1, 2);
    }
    if (level_time == 650)
    {
        ENY_spawnPopcorn(150, -40, 1, 1, 2);
    }

    if (level_time == 750)
    {
        ENY_spawnFloater(150, -35, 1, 0);
    }

    if (level_time == 800)
    {
        ENY_spawnFloater(100, -45, 2, 1);
        ENY_spawnFloater(200, -45, 2, 1);
    }

    if (level_time == 900)
    {
        ENY_spawnFloater(100, -45, 1, 1);
        ENY_spawnFloater(200, -45, 1, 1);
    }

    if (level_time == 950)
    {
        ENY_spawnFloater(150, -35, 2, 0);
    }
    if (level_time == 1150)
    {
        ENY_spawnPopcorn(70, -40, 0, 1, 2);
        ENY_spawnPopcorn(230, -40, 0, 1, 2);
    }
    if (level_time == 1175)
    {
        ENY_spawnPopcorn(80, -40, 0, 1, 2);
        ENY_spawnPopcorn(220, -40, 0, 1, 2);
    }
    if (level_time == 1200)
    {
        ENY_spawnPopcorn(90, -40, 0, 1, 2);
        ENY_spawnPopcorn(210, -40, 0, 1, 2);
    }
    if (level_time == 1220)
    {
        ENY_spawnPopcorn(100, -40, 0, 1, 2);
        ENY_spawnPopcorn(200, -40, 0, 1, 2);
    }

    if (level_time == 1450)
    {
        endCurrentLevel();
        PAL_fadeOutPalette(PAL3, 50, true);
    }
}

static void level_3(void)
{
    u32 level_time = getLevelTime();
    if (level_time < INTRO_LENGTH)
    {
        startNewLevel(level_time);
    }

    if (level_time == 325)
    {
        ENY_spawnAstroid(110, -30, 0, 1);
    }

    if (level_time == 350)
    {
        ENY_spawnAstroid(200, -30, 0, 1);
    }

    if (level_time == 400)
    {
        ENY_spawnAstroid(90, -35, 0, 2);
        ENY_spawnAstroid(210, -30, 0, 1);
    }

    if (level_time == 500)
    {
        ENY_spawnAstroid(100, -30, 0, 2);
        ENY_spawnAstroid(190, -45, 0, 1);        
    }

    if (level_time == 575)
    {
        ENY_spawnAstroid(55, -26, 0, 1);
        ENY_spawnAstroid(200, -31, 0, 1);
    }
    if (level_time == 650)
    {
        ENY_spawnAstroid(90, -30, 0, 1);
        ENY_spawnAstroid(210, -35, 0, 2);
    }

    if (level_time == 725)
    {
        ENY_spawnAstroid(65, -25, 0, 1);
        ENY_spawnAstroid(185, -55, 0, 2);
        ENY_spawnAstroid(210, -33, 0, 2);
    }

    if (level_time == 750)
    {
        ENY_spawnAstroid(115, -33, 0, 1);
    }

    if (level_time == 875)
    {
        ENY_spawnFloater(100, -35, 1, 0);
        ENY_spawnFloater(200, -35, 1, 0);
    }

    if (level_time == 950)
    {
        ENY_spawnFloater(150, -35, 1, 0);
    }

    if (level_time == 1100)
    {
        ENY_spawnPopcorn(150, -40, 0, 1, 2);
    }

    if (level_time == 1125)
    {
        ENY_spawnPopcorn(130, -40, 0, 1, 2);
        ENY_spawnPopcorn(170, -40, 0, 1, 2);
    }

    if (level_time == 1150)
    {
        ENY_spawnPopcorn(110, -40, 0, 1, 2);
        ENY_spawnPopcorn(190, -40, 0, 1, 2);
    }
    if (level_time == 1175)
    {
        ENY_spawnPopcorn(90, -40, 0, 1, 2);
        ENY_spawnPopcorn(210, -40, 0, 1, 2);
        ENY_spawnPopcorn(150, -40, 0, 1, 2);
    }

    if (level_time == 1300)
    {
        ENY_spawnAstroid(50, -25, 0, 1);
        ENY_spawnAstroid(150, -55, 0, 2);        
    }

    if (level_time == 1375)
    {
        ENY_spawnFloater(100, -45, 2, 1);
        ENY_spawnFloater(200, -45, 2, 1);
    }

    if (level_time == 1425)
    {
        ENY_spawnFloater(50, -45, 2, 1);
        ENY_spawnFloater(250, -45, 2, 1);
    }

    if (level_time == 1325)
    {
        ENY_spawnAstroid(202, -33, 0, 1);
    }

    if (level_time == 1375)
    {
        ENY_spawnAstroid(54, -45, 0, 2);
        ENY_spawnAstroid(125, -75, 0, 1);
    }

    if (level_time == 1425)
    {
        ENY_spawnAstroid(92, -45, 0, 2);
        ENY_spawnAstroid(176, -75, 0, 1);
    }

    if (level_time == 1475)
    {
        ENY_spawnAstroid(44, -45, 0, 2);
        ENY_spawnAstroid(245, -33, 0, 2);
    }

    if (level_time == 1500)
    {
        ENY_spawnAstroid(75, -35, 0, 3);
    }

    if (level_time == 1550)
    {
        ENY_spawnAstroid(200, -35, 0, 3);
    }

    if (level_time == 1600)
    {
        ENY_spawnAstroid(120, -35, 0, 2);
    }

    if (level_time == 1650)
    {
        endCurrentLevel();
    }
}

static void level_4(void)
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
    case 4:
        level_3();
        break;
    }
}

void SCR_init(void)
{
    addTickFunc(runscript, true);
}