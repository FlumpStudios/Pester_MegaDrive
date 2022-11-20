#include "gamestate.h"
#include "ui.h"


typedef struct gs
{
    bool is_game_playing;
    u8 current_game_state;
    u32 score;
    u32 high_score;
    u32 level_time;
    u8 current_level;
    u8 current_lives;
    bool isGamePaused;
} Gamestate_t;

static Gamestate_t *gamestate = NULL;

void resetGame(void)
{
    gamestate->score = 0;
    gamestate->level_time = 0;
    gamestate->current_game_state = GAME_STATE_MENU;
    gamestate->is_game_playing = false;
    gamestate->high_score = 0;
    gamestate->current_level = STARTING_LEVEL;
    gamestate->current_lives = STARTING_LIVES;
    gamestate->isGamePaused = false;
}

bool ST_is_game_paused(void)
{
    return gamestate->isGamePaused;
}

void ST_set_is_game_paused(bool isPaused)
{
    gamestate->isGamePaused = isPaused;
}


void removeLife(void)
{
    gamestate->current_lives--;
    UI_updateLivesText();
}

void addLife(void)
{
    gamestate->current_lives++;
    UI_updateLivesText();
}

u8 getLivesCount(void)
{
    return gamestate->current_lives;
}



void resetLevelTime(void)
{
    gamestate->level_time = 0;
}

int getScore(void)
{
    return gamestate->score;
}


u8 getCurrentLevel(void)
{
    return gamestate->current_level;
}

void increaseCurrentLevel(void)
{
    gamestate->current_level++;
}

int getHighScore(void)
{
    return gamestate->high_score;
}

u8 getGameState(void)
{
    return gamestate->current_game_state;
}


bool isGamePlaying(void)
{
    return gamestate->is_game_playing;
}

u32 getLevelTime(void)
{
    return gamestate->level_time;
}


void increaseScore(u32 score)
{
    gamestate->score += score;
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

void endGame(void)
{
    UI_drawCentredText(MSG_RESET);
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

void restartGame(void)
{
    gamestate->current_lives = STARTING_LIVES;
    setGamePlaying(true);
    resetPlayer();
    ENY_resetAllEnemies();
    resetScore();   
    resetLevelTime();
    resetCurrentLevel();
    VDP_clearTextArea(0, 10, 40, 10);
    UI_init();
}


void startGame(void)
{
    UI_clearCentredText();
    PAL_fadeInPalette(PAL3, introImage.palette->data, 150, true);
    BCK_draw_starfield();
    setGameState(GAME_STATE_GAME);
    PLY_init();
    VX_init();
    ENY_init();
    UI_init();

}

void ST_update(void)
{
    if (isGamePlaying())
    {
        tickLevelTime();
    }
}

void ST_init(void)
{
    gamestate = NULL;
    gamestate = MEM_alloc(sizeof(Gamestate_t));

    if (gamestate != NULL)
    {
        resetGame();
    }

    addTickFunc(ST_update, false);
}

void destructState(void)
{
    MEM_free(gamestate);
}
