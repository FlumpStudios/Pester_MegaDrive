#include "gamestate.h"

typedef struct gs
{
    bool is_game_playing;
    u8 current_game_state;
    u8 game_time_mod;
    u32 score;
    u32 play_time;
    u32 game_time;
} Gamestate_t;

static Gamestate_t *gamestate = NULL;

void resetGame(void)
{
    gamestate->score = 0;
    gamestate->play_time = 0;
    gamestate->current_game_state = MENU;
    gamestate->is_game_playing = false;
    gamestate->game_time = 0;
    gamestate->game_time_mod = 1;    
}

void resetGameTime(void)
{
    gamestate->game_time = 0;
}

void initiateGameState(void)
{
    gamestate = (Gamestate_t *)MEM_alloc(sizeof(Gamestate_t));
    resetGame();
}

int getScore(void)
{
    return gamestate->score;
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