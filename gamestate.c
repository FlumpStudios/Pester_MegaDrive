#include "gamestate.h"
#include "ui.h"
#include "audio.h"

typedef struct gs
{
    bool is_game_playing;
    u8 current_game_state;
    u32 score;
    u32 high_score;
    u32 level_time;
    u32 chain;
    u8 current_level;
    u8 current_lives;
    bool isGamePaused;
} Gamestate_t;

static Gamestate_t *gamestate = NULL;

void initiateGameState(void)
{
    gamestate->chain = 0;
    gamestate->score = 0;
    gamestate->level_time = 0;
    gamestate->current_game_state = GAME_STATE_MENU;
    gamestate->is_game_playing = false;
    gamestate->high_score = 0;
    gamestate->current_level = STARTING_LEVEL;
    gamestate->current_lives = STARTING_LIVES;
    gamestate->isGamePaused = false;
}

bool GST_is_game_paused(void)
{
    return gamestate->isGamePaused;
}

void GST_set_is_game_paused(bool isPaused)
{
    gamestate->isGamePaused = isPaused;
}

void GST_removeLife(void)
{
    gamestate->current_lives--;
    UI_updateLivesText();
}

void GST_addLife(void)
{
    gamestate->current_lives++;
    UI_updateLivesText();
}

u8 GST_getLivesCount(void)
{
    return gamestate->current_lives;
}

void GST_resetLevelTime(void)
{
    gamestate->level_time = 0;
}

u32 GST_getScore(void)
{
    return gamestate->score;
}

u8 GST_getCurrentLevel(void)
{
    return gamestate->current_level;
}

void GST_increaseCurrentLevel(void)
{
    gamestate->current_level++;
}

u32 GST_getHighScore(void)
{
    return gamestate->high_score;
}

u32 GST_getChain(void)
{
    return gamestate->chain;
}

void GST_increaseChain(u32 chain)
{
    gamestate->chain += chain;
}

void GST_resetChain(void)
{
    if (gamestate->chain > 0)
    {
        AUD_play_lost_chain();
    }
    gamestate->chain = 0;
}

u8 GST_getGameState(void)
{
    return gamestate->current_game_state;
}

bool GST_isGamePlaying(void)
{
    return gamestate->is_game_playing;
}

u32 GST_getLevelTime(void)
{
    return gamestate->level_time;
}

void GST_increaseScore(u32 score)
{
    gamestate->score += (score + (gamestate->chain * 10));
}

void resetScore(void)
{
    gamestate->score = 0;
}

void setGameState(u8 gameState)
{
    gamestate->current_game_state = gameState;
}

void setGamePlaying(bool isGamePlaying)
{
    gamestate->is_game_playing = isGamePlaying;
}

void tickLevelTime(void)
{
    if (!gamestate->isGamePaused)
    {
        gamestate->level_time += 1;
    }
}

void GST_endGame(void)
{
    char scoreTextBuffer[30]; 

    sprintf(scoreTextBuffer, "%s %lu", MSG_RESET, gamestate->score);

    UI_drawCentredText(scoreTextBuffer);
    setGamePlaying(false);
    if (gamestate->score > gamestate->high_score)
    {
        gamestate->high_score = gamestate->score;
        UI_updateHighScoreDisplay();
    }
}

void resetCurrentLevel(void)
{
    gamestate->current_level = STARTING_LEVEL;
}

void GST_restartGame(void)
{
    gamestate->current_lives = STARTING_LIVES;
    setGameState(GAME_STATE_GAME);
    setGamePlaying(true);
    PLY_resetPlayer();
    ENY_resetAllEnemies();
    resetScore();
    GST_resetLevelTime();
    resetCurrentLevel();
    VDP_clearTextArea(0, 10, 40, 10);
    BCK_draw_starfield();
    AUD_play_level1_music();
    PAL_fadeInPalette(PAL3, introImage.palette->data, 150, true);
    UI_clearCentredText();
    UI_init();
}

void GST_startGame(void)
{
    AUD_play_start();
    PLY_init();
    ENY_init();
    GST_restartGame();
}



// TODO: Move to a better place, doesn't really fit in game state
void GST_resetPalletes(void)
{
    // Text
    VDP_setPalette(PAL0, palette_grey);

    // VFX and Boss switching
    VDP_setPalette(PAL1, imgexplo.palette->data);

    // Enemies
    VDP_setPalette(PAL2, bird.palette->data);

    // Background
    VDP_setPalette(PAL3, palette_black);

	VDP_setTextPlan(PLAN_A);
}

void GST_setUpMainMenu(void)
{
    AUD_play_menu_music();
    setGameState(GAME_STATE_MENU);
    GST_resetPalletes();
    BCK_draw_title_screen();
    PAL_fadeInPalette(PAL3, introImage.palette->data, 100, false);
    CTR_set_locked_controls(false);
}

void GST_freeGameResources(void)
{
    ENY_destruct_enemies();
    ENY_destruct_bullets();
    PLY_destructPlayer();
    UI_destruct();    
}

void ST_update(void)
{
    if (GST_isGamePlaying())
    {
        tickLevelTime();
    }
}

void GST_init(void)
{
    gamestate = NULL;
    gamestate = MEM_alloc(sizeof(Gamestate_t));

    if (gamestate != NULL)
    {
        initiateGameState();
    }

    static bool tickAdded = false;
    if(!tickAdded)
    {
        addTickFunc(ST_update, false);
        tickAdded = true;
    }
}

void GST_destructState(void)
{
    MEM_free(gamestate);
    gamestate = NULL;
}
