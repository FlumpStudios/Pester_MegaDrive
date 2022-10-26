#include "game_script.h"
#include "enemies.h"
#include "game_update_observable.h"
#include "gamestate.h"

void runscript(void)
{
    u32 game_time = getGameTime();
    if (game_time == 10)
    {
          ENY_setGrabberCount(2);
    }
    if (game_time == 400)
    {
        ENY_setGrabberCount(4);
    }

    if (game_time == 1000)
    {
        ENY_setGrabberCount(0);
        ENY_setBirdCount(3);        
    }

    if (game_time == 1500)
    {
        ENY_setBirdCount(2);
        ENY_setGrabberCount(4);
    }  
}

void SCR_init(void)
{
    addTickFunc(runscript, true);
}