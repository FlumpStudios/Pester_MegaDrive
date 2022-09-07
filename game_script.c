#include "game_script.h"
#include "enemies.h"
#include "game_update_observable.h"
#include "gamestate.h"

void runscript(void)
{
    u32 game_time = getGameTime();
    if (game_time > 100)
    {
        
    }
}

void SCR_init(void)
{
    addTickFunc(runscript, true);
}