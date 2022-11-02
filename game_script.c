#include "game_script.h"
#include "enemies.h"
#include "game_update_observable.h"
#include "gamestate.h"

#define MAX_LEVEL_DIDGETS 3
#define INTRO_LENGTH 300

static void endCurrentLevel(void)
{
    increaseCurrentLevel();
    resetLevelTime();
}

static void displayNewLevelText(void)
{
    char levelTextBuffer[6 + MAX_LEVEL_DIDGETS] = "Level ";
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

static void level_1(void)
{
    u32 level_time = getLevelTime();
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

    if (level_time == 300)
    {
        ENY_spawnGrabber(-50, -10, 1, 1);
        ENY_spawnGrabber(300, -2, -3, 1);
    }
    if (level_time == 400)
    {
        ENY_spawnGrabber(-50, -20, 3, 1);
        ENY_spawnGrabber(300, -15, -2, 1);
    }

    if (level_time == 500)
    {
        ENY_spawnBird(150, -10, 1, 2);
    }

    if (level_time == 700)
    {
        endCurrentLevel();
    }
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
        ENY_spawnAstroid(100, -10, 0, 2);
        ENY_spawnAstroid(190, -30, 0, 1);
        ENY_spawnAstroid(150, -30, 0, 2);
    }

    if (level_time == 350)
    {
        ENY_spawnAstroid(55, -10, 0, 3);
        ENY_spawnAstroid(120, -30, 0, 2);
        ENY_spawnAstroid(200, -30, 0, 2);
    }
    if (level_time == 400)
    {
        ENY_spawnAstroid(90, -20, 0, 2);
        ENY_spawnAstroid(124, -60, 0, 3);
        ENY_spawnAstroid(210, -30, 0, 2);
    }

    if (level_time == 450)
    {
        ENY_spawnAstroid(40, -10, 0, 1);
        ENY_spawnAstroid(160, -250, 0, 3);
        ENY_spawnAstroid(220, -15, 0, 2);
    }

    if (level_time == 500)
    {
        ENY_spawnAstroid(100, -10, 0, 2);
        ENY_spawnAstroid(190, -30, 0, 1);
        ENY_spawnAstroid(150, -30, 0, 2);
        ENY_spawnGrabber(-50, -10, 1, 1);
        ENY_spawnGrabber(300, -2, -3, 1);
    }

    if (level_time == 550)
    {
        ENY_spawnAstroid(55, -10, 0, 3);
        ENY_spawnAstroid(120, -30, 0, 2);
        ENY_spawnAstroid(200, -30, 0, 3);
        ENY_spawnGrabber(-50, -20, 3, 1);
        ENY_spawnGrabber(300, -15, -2, 1);
    }
    if (level_time == 600)
    {
        ENY_spawnAstroid(90, -20, 0, 2);
        ENY_spawnAstroid(124, -60, 0, 3);
        ENY_spawnAstroid(210, -30, 0, 2);
    }

    if (level_time == 650)
    {
        ENY_spawnAstroid(40, -10, 0, 3);
        ENY_spawnAstroid(160, -250, 0, 3);
        ENY_spawnAstroid(220, -15, 0, 2);
        ENY_spawnGrabber(-50, -20, 3, 1);
        ENY_spawnGrabber(300, -15, -2, 1);
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