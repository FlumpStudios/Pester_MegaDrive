#include "game_script.h"
#include "enemies.h"
#include "game_update_observable.h"
#include "gamestate.h"
#include "controls.h"
#include "player.h"
#include "common.h"
#include "audio.h"

#define MAX_LEVEL_DIDGETS 3
#define INTRO_LENGTH 300
#define WARNING_MESSAGE_LENGTH 400
#define FIRST_FRAME 1

void SCR_end_current_level(void)
{
    ENY_destruct_enemies();
    ENY_destruct_bullets();
    AUD_play_wave_complete();
    GST_increaseCurrentLevel();
    GST_resetLevelTime();
}

// TODO: Move to UI
static void displayNewLevelText(void)
{
    char levelTextBuffer[6 + MAX_LEVEL_DIDGETS] = "Wave ";
    char levelNumberBuffer[MAX_LEVEL_DIDGETS];

    sprintf(levelNumberBuffer, "%d", GST_getCurrentLevel());
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
        PLY_moveDown(false);
    }

    if (level_time == 150)
    {
        PLY_haltY();
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
        AUD_play_alarm();
        UI_display_warning();
    }
    else if (levelTime % 25 == 0)
    {
        UI_clear_warning();
    }
}

static void level_1(void)
{
    u32 level_time = GST_getLevelTime();

    if (level_time == FIRST_FRAME)
    {
        ENY_createPopcornPool(8);
    }

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
        ENY_spawnPopcorn(75, -40, 1, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(200, -40, 1, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 350)
    {
        ENY_spawnPopcorn(75, -40, 1, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(200, -40, 1, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 380)
    {
        ENY_spawnPopcorn(75, -40, 1, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(200, -40, 1, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 410)
    {
        ENY_spawnPopcorn(75, -40, 1, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(200, -40, 1, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 540)
    {
        ENY_spawnPopcorn(150, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 570)
    {
        ENY_spawnPopcorn(125, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(175, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 600)
    {
        ENY_spawnPopcorn(150, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 720)
    {
        ENY_spawnPopcorn(150, -40, 2, 3, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 740)
    {
        ENY_spawnPopcorn(175, -40, 2, 3, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(125, -40, 2, 3, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 760)
    {
        ENY_spawnPopcorn(200, -40, 2, 3, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(100, -40, 2, 3, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 780)
    {
        ENY_spawnPopcorn(225, -40, 2, 3, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(75, -40, 2, 3, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 1000)
    {
        ENY_spawnPopcorn(150, -40, -5, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(150, -40, 5, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 1030)
    {
        ENY_spawnPopcorn(150, -40, -5, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(150, -40, 5, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 1060)
    {
        ENY_spawnPopcorn(150, -40, -5, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(150, -40, 5, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 1350)
    {
        SCR_end_current_level();
    }

    // if (level_time == 500)
    // {
    //     ENY_spawnBird(150, -25, 1, 2);
    // }
}

static void level_2(void)
{
    u32 level_time = GST_getLevelTime();

    if (level_time == FIRST_FRAME)
    {
        ENY_createFloaterPool(6);
        ENY_createPopcornPool(8);
        ENY_createCircleBulletPool(8);
    }

    if (level_time < INTRO_LENGTH)
    {
        startNewLevel(level_time);
    }

    if (level_time == 300)
    {
        ENY_spawnFloater(100, -35, 1, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(200, -75, 1, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(100, -105, 1, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }
    if (level_time == 550)
    {
        ENY_spawnPopcorn(150, -40, 2, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 575)
    {
        ENY_spawnPopcorn(150, -40, 2, 2, VARIATION_POPCORN_CENTRE);
    }
    if (level_time == 600)
    {
        ENY_spawnPopcorn(150, -40, 2, 2, VARIATION_POPCORN_CENTRE);
    }
    if (level_time == 625)
    {
        ENY_spawnPopcorn(150, -40, 2, 2, VARIATION_POPCORN_CENTRE);
    }
    if (level_time == 650)
    {
        ENY_spawnPopcorn(150, -40, 2, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 750)
    {
        ENY_spawnFloater(150, -35, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }

    if (level_time == 800)
    {
        ENY_spawnFloater(100, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(200, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }

    if (level_time == 900)
    {
        ENY_spawnFloater(100, -45, 1, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(200, -45, 1, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }

    if (level_time == 950)
    {
        ENY_spawnFloater(150, -35, 2, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }
    if (level_time == 1150)
    {
        ENY_spawnPopcorn(70, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(230, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }
    if (level_time == 1175)
    {
        ENY_spawnPopcorn(80, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(220, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }
    if (level_time == 1200)
    {
        ENY_spawnPopcorn(90, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(210, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }
    if (level_time == 1220)
    {
        ENY_spawnPopcorn(100, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(200, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 1450)
    {
        SCR_end_current_level();
    }
}

static void level_3(void)
{
    u32 level_time = GST_getLevelTime();

    if (level_time == FIRST_FRAME)
    {
        ENY_createAstroidPool(6);
        ENY_createFloaterPool(6);
        ENY_createPopcornPool(8);
        ENY_createCircleBulletPool(8);
    }

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
        ENY_spawnFloater(100, -35, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(200, -35, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }

    if (level_time == 950)
    {
        ENY_spawnFloater(150, -35, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }

    if (level_time == 1100)
    {
        ENY_spawnPopcorn(150, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 1125)
    {
        ENY_spawnPopcorn(130, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(170, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 1150)
    {
        ENY_spawnPopcorn(110, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(190, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }
    if (level_time == 1175)
    {
        ENY_spawnPopcorn(90, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(210, -40, 0, 2, VARIATION_POPCORN_CENTRE);
        ENY_spawnPopcorn(150, -40, 0, 2, VARIATION_POPCORN_CENTRE);
    }

    if (level_time == 1300)
    {
        ENY_spawnAstroid(50, -25, 0, 1);
        ENY_spawnAstroid(150, -55, 0, 2);
    }

    if (level_time == 1375)
    {
        ENY_spawnFloater(100, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(200, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }

    if (level_time == 1425)
    {
        ENY_spawnFloater(50, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(250, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
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
        SCR_end_current_level();
    }
}

static void level_4(void)
{
    u32 level_time = GST_getLevelTime();

    if (level_time == FIRST_FRAME)
    {
        ENY_createGrabberPool(6);
        ENY_createFloaterPool(6);
        ENY_createPopcornPool(10);
        ENY_createCircleBulletPool(8);
        ENY_createRocketBulletPool(6);
    }

    if (level_time < INTRO_LENGTH)
    {
        startNewLevel(level_time);
    }

    if (level_time == 300)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }

    if (level_time == 306)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 312)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 318)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 324)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }

    if (level_time == 414)
    {
        ENY_spawnPopcorn(180, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 420)
    {
        ENY_spawnPopcorn(180, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 426)
    {
        ENY_spawnPopcorn(180, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 432)
    {
        ENY_spawnPopcorn(180, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 438)
    {
        ENY_spawnPopcorn(180, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }

    if (level_time == 494)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 500)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 506)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 512)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 518)
    {
        ENY_spawnPopcorn(90, -40, 1, 2, VARIATION_SNAKE_CENTRE);
    }

    if (level_time == 575)
    {
        ENY_spawnGrabber(-50, 50, 1, 0);
        ENY_spawnGrabber(340, 50, -1, 0);
    }

    if (level_time == 615)
    {
        ENY_spawnGrabber(-50, -35, 2, 1);
        ENY_spawnGrabber(340, -35, -2, 1);
    }

    if (level_time == 900)
    {
        ENY_spawnGrabber(-50, 25, 2, 0);
        ENY_spawnGrabber(340, 25, -2, 0);
    }

    if (level_time == 1050)
    {
        ENY_spawnFloater(150, -35, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }

    if (level_time == 1100)
    {
        ENY_spawnFloater(100, -45, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(200, -45, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }

    if (level_time == 1250)
    {
        ENY_spawnGrabber(-50, 25, 2, 0);
        ENY_spawnGrabber(340, 25, -2, 0);
    }

    if (level_time == 1350)
    {
        ENY_spawnGrabber(-50, 50, 2, 0);
        ENY_spawnGrabber(340, 50, -2, 0);
    }

    if (level_time == 1450)
    {
        ENY_spawnGrabber(-50, 75, 1, 0);
        ENY_spawnGrabber(340, 75, -1, 0);
    }

    if (level_time == 1400)
    {
        ENY_spawnFloater(80, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(220, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }

    if (level_time == 1500)
    {
        ENY_spawnFloater(110, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(190, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }

    if (level_time == 1600)
    {
        ENY_spawnFloater(150, -45, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }

    if (level_time == 1750)
    {
        ENY_spawnPopcorn(150, -40, 1, 3, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 1755)
    {
        ENY_spawnPopcorn(150, -40, 1, 3, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 1760)
    {
        ENY_spawnPopcorn(150, -40, 1, 3, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 1765)
    {
        ENY_spawnPopcorn(150, -40, 1, 3, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 1770)
    {
        ENY_spawnPopcorn(150, -40, 1, 3, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 1775)
    {
        ENY_spawnPopcorn(150, -40, 1, 3, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 1780)
    {
        ENY_spawnPopcorn(150, -40, 1, 3, VARIATION_SNAKE_CENTRE);
    }
    if (level_time == 1785)
    {
        ENY_spawnPopcorn(150, -40, 1, 3, VARIATION_SNAKE_CENTRE);
    }

    if (level_time == 1990)
    {
        SCR_end_current_level();
    }
    // ENY_spawnBouncer(80, -50, 5, 1, 150);
    // ENY_spawnBouncer(200, -50, 5, 1, 150);
}

static void level_5(void)
{
    u32 level_time = GST_getLevelTime();
    if (level_time == FIRST_FRAME)
    {
        ENY_createGrabberPool(6);
        ENY_createFloaterPool(6);
        ENY_createCircleBulletPool(10);
        ENY_createRocketBulletPool(6);
        ENY_createAstroidPool(6);
    }
    if (level_time < INTRO_LENGTH)
    {
        startNewLevel(level_time);
    }

    if (level_time == 300)
    {
        ENY_spawnGrabber(-50, 30, 2, 0);
    }

    if (level_time == 350)
    {
        ENY_spawnGrabber(300, 60, -2, 0);
    }

    if (level_time == 400)
    {
        ENY_spawnGrabber(-50, 30, 2, 0);
    }

    if (level_time == 450)
    {
        ENY_spawnGrabber(300, 60, -2, 0);
    }

    if (level_time == 500)
    {
        ENY_spawnGrabber(-50, 30, 2, 0);
    }
    if (level_time == 510)
    {
        ENY_spawnAstroid(75, -45, 0, 2);
        ENY_spawnAstroid(220, -45, 0, 1);
    }

    if (level_time == 540)
    {
        ENY_spawnAstroid(45, -30, 0, 2);
        ENY_spawnAstroid(190, -55, 0, 1);
    }

    if (level_time == 550)
    {
        ENY_spawnGrabber(300, 60, -2, 0);
    }

    if (level_time == 600)
    {
        ENY_spawnGrabber(-50, 50, 2, 0);
    }

    if (level_time == 535)
    {
        ENY_spawnAstroid(185, -45, 0, 2);
        ENY_spawnAstroid(135, -40, 0, 1);
    }

    if (level_time == 650)
    {
        ENY_spawnGrabber(300, 60, -2, 0);
    }

    if (level_time == 750)
    {
        ENY_spawnAstroid(130, -35, 0, 2);
        ENY_spawnAstroid(55, -60, 0, 1);
    }

    if (level_time == 700)
    {
        ENY_spawnGrabber(-50, 30, 2, 0);
    }

    if (level_time == 800)
    {
        ENY_spawnGrabber(300, 60, -2, 0);
    }

    if (level_time == 850)
    {
        ENY_spawnGrabber(-50, 30, 2, 0);
    }

    if (level_time == 950)
    {
        ENY_spawnAstroid(45, -25, 0, 2);
        ENY_spawnAstroid(140, -68, 0, 2);
        ENY_spawnAstroid(220, -45, 0, 2);
    }

    if (level_time == 1000)
    {
        ENY_spawnFloater(150, -35, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(100, -65, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(200, -65, 2, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }

    if (level_time == 1150)
    {
        ENY_spawnFloater(150, -30, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(130, -50, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(170, -50, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(150, -70, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }

    if (level_time == 1400)
    {
        ENY_spawnAstroid(80, -25, 0, 1);
        ENY_spawnAstroid(220, -90, 0, 2);
    }

    if (level_time == 1450)
    {
        ENY_spawnAstroid(60, -25, 0, 2);
        ENY_spawnAstroid(180, -75, 0, 1);
    }

    if (level_time == 1400)
    {
        ENY_spawnAstroid(90, -25, 0, 2);
        ENY_spawnAstroid(150, -70, 0, 1);
    }

    if (level_time == 1550)
    {
        ENY_spawnAstroid(115, -36, 0, 1);
        ENY_spawnAstroid(222, -90, 0, 1);
    }

    if (level_time == 1600)
    {
        ENY_spawnAstroid(150, -40, 0, 1);
    }

    if (level_time == 1500)
    {
        ENY_spawnFloater(150, -35, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(100, -50, 1, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(200, -50, 1, VARIATION_FLOATER_WITH_SIDE_BULLETS);
    }

    if (level_time == 1650)
    {
        ENY_spawnFloater(150, -35, 1, VARIATION_FLOATER_WITH_SIDE_BULLETS);
        ENY_spawnFloater(100, -50, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(200, -50, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }

    if (level_time == 1850)
    {
        ENY_spawnGrabber(300, 60, -2, 0);
    }

    if (level_time == 1900)
    {
        ENY_spawnGrabber(300, 60, -2, 0);
    }

    if (level_time == 1950)
    {
        ENY_spawnGrabber(-50, 30, 2, 0);
    }

    if (level_time == 2000)
    {
        ENY_spawnGrabber(300, 60, -2, 0);
    }

    if (level_time == 2050)
    {
        ENY_spawnGrabber(-50, 30, 2, 0);
    }

    if (level_time == 1900)
    {
        ENY_spawnFloater(150, -30, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(130, -50, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(170, -50, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
        ENY_spawnFloater(150, -70, 1, VARIATION_FLOATER_WITH_FORKED_BULLETS);
    }

    if (level_time == 2250)
    {
        SCR_end_current_level();
    }

    // Side snake
    // if (level_time == 300)
    // {
    //     ENY_spawnPopcorn(-30, 60, 3, 1, VARIATION_SNAKE_RIGHT);
    // }

    // if (level_time == 306)
    // {
    //     ENY_spawnPopcorn(-30, 60, 3, 1, VARIATION_SNAKE_RIGHT);
    // }
    // if (level_time == 312)
    // {
    //     ENY_spawnPopcorn(-30, 60, 3, 1, VARIATION_SNAKE_RIGHT);
    // }
    // if (level_time == 318)
    // {
    //     ENY_spawnPopcorn(-30, 60, 3, 1, VARIATION_SNAKE_RIGHT);
    // }
    // if (level_time == 324)
    // {
    //     ENY_spawnPopcorn(-30, 60, 3, 1, VARIATION_SNAKE_RIGHT);
    // }
}

static void level_6(void)
{
    u32 level_time = GST_getLevelTime();
    if (level_time == FIRST_FRAME)
    {
        ENY_createRocketBulletPool(8);
        ENY_createCircleBulletPool(12);
    }

    if (level_time < INTRO_LENGTH)
    {
        startNewLevel(level_time);
    }
    if (level_time > INTRO_LENGTH && level_time < (WARNING_MESSAGE_LENGTH + INTRO_LENGTH))
    {
        showWarning(level_time);
    }

    if (level_time == 200)
    {
        VDP_setPalette(PAL1, boss1Left.palette->data);
    }
    if (level_time == 500)
    {
        ENY_spawn_boss_1();
    }
}

static void level_7(void)
{
    u32 level_time = GST_getLevelTime();

    if (level_time < INTRO_LENGTH)
    {
        startNewLevel(level_time);
    }

    if (level_time == 50)
    {
        PAL_fadeOutPalette(PAL3, 150, true);
    }

    if (level_time == 300)
    {
        BCK_draw_moon();
        PAL_fadeInPalette(PAL3, moon.palette->data, 150, true);
    }
}

void runscript(void)
{
    switch (GST_getCurrentLevel())
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
        level_4();
        break;
    case 5:
        level_5();
        break;
    case 6:
        level_6();
        break;
    case 7:
        level_7();
        break;
    }
}

void SCR_init(void)
{
    static bool tickAdded = false;
    if (!tickAdded)
    {
        addTickFunc(runscript, true);
        tickAdded = true;
    }
}