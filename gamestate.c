#include "gamestate.h"

typedef struct gs
{
    bool is_game_playing;
    u8 current_game_state;
    u8 game_time_mod;
    u32 score;
    u32 high_score;
    u32 play_time;
    u32 game_time;
    u16 current_level;
} Gamestate_t;

static Gamestate_t *gamestate = NULL;

void resetGame(void)
{
    gamestate->score = 0;
    gamestate->play_time = 0;
    gamestate->current_game_state = GAME_STATE_MENU;
    gamestate->is_game_playing = false;
    gamestate->game_time = 0;
    gamestate->game_time_mod = 1;
    gamestate->high_score = 0; 
    gamestate->current_level = 0;
}
 
void resetGameTime(void)
{
    gamestate->game_time = 0;
}

int getScore(void)
{
    return gamestate->score;
}

u16 getCurrentLevel(void)
{
    return gamestate->current_level;
}

void increaseCurrentLevel(void)
{
    gamestate->current_level ++;
}

int getHighScore(void)
{
    return gamestate->high_score;
}

u8 getGameState(void)
{
    return gamestate->current_game_state;
}

u8 getTimeMod(void)
{
    return gamestate->game_time_mod;
}

bool isGamePlaying(void)
{
    return gamestate->is_game_playing;
}

u32 getPlayTime(void)
{
    return gamestate->play_time;
}

u32 getGameTime(void)
{
    return gamestate->game_time;
}

void increaseScore(int score)
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

void setTimeMod(u8 timeMod)
{
    gamestate->game_time_mod = timeMod;
}

void setGamePlaying(bool isGamePlaying)
{
    gamestate->is_game_playing = isGamePlaying;
}

void tickPlayTime(void)
{
    gamestate->play_time++;
}

void tickGameTime(void)
{
    gamestate->game_time += gamestate->game_time_mod;
}

void endGame(void)
{
    UI_drawCentredText(MSG_RESET);
    setGamePlaying(false);
    if(gamestate->score > gamestate->high_score)
    {
        gamestate->high_score = gamestate->score;
        UI_updateHighScoreDisplay();
    }
}

void restartGame(void)
{
    setGamePlaying(true);
    resetPlayer();
    ENY_resetAllEnemies();
    resetScore();
    UI_updateScoreDisplay();
    resetGameTime();
    VDP_clearTextArea(0, 10, 40, 10);
}

void startGame(void)
{
    BCK_init();
    PLY_init();
    VX_init();
    ENY_init();
    UI_drawHud();
    setGameState(GAME_STATE_GAME);
}

void ST_update(void)
{
    tickPlayTime();
    if(isGamePlaying())
    {
        tickGameTime();
    }
}

void ST_init(void)
{
    if (gamestate == NULL)
    {
        gamestate = MEM_alloc(sizeof(Gamestate_t));
    }
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
