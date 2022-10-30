#include "game_script.h"
#include "enemies.h"
#include "game_update_observable.h"
#include "gamestate.h"

#define MAX_LEVEL_DIDGETS 3

void updateAndDisplayNewLevel(void)
{
    char levelTextBuffer[6 + MAX_LEVEL_DIDGETS] = "Level ";
    char levelNumberBuffer[MAX_LEVEL_DIDGETS];

    increaseCurrentLevel();

    sprintf(levelNumberBuffer, "%d", getCurrentLevel());
    strcat(levelTextBuffer, levelNumberBuffer);
    UI_drawCentredText(levelTextBuffer);
}

void runscript(void)
{
    u32 game_time = getGameTime();
    if (game_time == 10)
    {
        UI_drawCentredText("Get Ready");
    }

    if (game_time == 10)
    {
        UI_drawCentredText("Get Ready");
    }

    if (game_time == 100)
    {
        updateAndDisplayNewLevel();
    }

    if (game_time == 200)
    {
        UI_clearCentredText();
    }

    if (game_time == 300)
    {
        ENY_spawnGrabber(-50, -10, 1, 1);
        ENY_spawnGrabber(300, -2, -3, 1);
    }
    if (game_time == 400)
    {
        ENY_spawnGrabber(-50, -20, 3, 1);
        ENY_spawnGrabber(300, -15, -2, 1);
    }

    if (game_time == 500)
    {
        ENY_spawnBird(150, -10, 1, 2);
    }

    if (game_time == 600)
    {
        UI_drawCentredText("Wave complete");
    }

    if (game_time == 700)
    {
        updateAndDisplayNewLevel();
    }

    if (game_time == 800)
    {
        UI_clearCentredText();
    }
}

void SCR_init(void)
{
    addTickFunc(runscript, true);
}