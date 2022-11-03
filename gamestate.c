#include "gamestate.h"
#include "ui.h"
#define STARTING_LEVEL 1
#define STARTING_LIVES 2

typedef struct gs
{
    bool is_game_playing;
    u8 current_game_state;
    u8 game_time_mod;
    u32 score;
    u32 high_score;
    u32 level_time;
    u32 game_time;
    u8 current_level;
    u8 current_lives;
} Gamestate_t;

static Gamestate_t *gamestate = NULL;

void resetGame(void)
{
    gamestate->score = 0;
    gamestate->level_time = 0;
    gamestate->current_game_state = GAME_STATE_MENU;
    gamestate->is_game_playing = false;
    gamestate->game_time = 0;
    gamestate->game_time_mod = 1;
    gamestate->high_score = 0; 
    gamestate->current_level = STARTING_LEVEL;
    gamestate->current_lives = STARTING_LIVES;
}
 
void removeLife(void)
{
    gamestate->current_lives --;
    UI_updateLivesText();
}

void addLife(void)
{
    gamestate->current_lives ++;
    UI_updateLivesText();
}

u8 getLivesCount(void)
{
    return gamestate->current_lives;
}

void resetGameTime(void)
{
    gamestate->game_time = 0;
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

u32 getLevelTime(void)
{
    return gamestate->level_time;
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

void tickLevelTime(void)
{
    gamestate->level_time +=  gamestate->game_time_mod;
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
    UI_updateScoreDisplay();
    resetGameTime();
    resetLevelTime();
    resetCurrentLevel();
    VDP_clearTextArea(0, 10, 40, 10);
    UI_updateLivesText();
}

void startGame(void)
{
    BCK_init_starfield();
    PLY_init();
    VX_init();
    ENY_init();
    UI_drawHud();
    setGameState(GAME_STATE_GAME);    
    UI_updateLivesText();
}

void ST_update(void)
{
    if(isGamePlaying())
    {
        tickLevelTime();
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
