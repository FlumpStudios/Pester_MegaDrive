#include "game_update_observable.h"
#include "gamestate.h"

#define FUNC_LIST_CAPACITY 10

typedef struct { 
 bool gameplay_exclusive;
 tickFunction run;
} Func_t;

static Func_t func_list[FUNC_LIST_CAPACITY];
static u16 cur_index = 0;

void addTickFunc(tickFunction f, bool gameplay_only)
{
    if(cur_index < FUNC_LIST_CAPACITY)
    { 
        func_list[cur_index].run = f;
        func_list[cur_index].gameplay_exclusive = gameplay_only;
        cur_index++;
    }
}

void runTickFunctions(void)
{    
    for (u16 i = 0; i < cur_index; i++)
    {
        if(func_list[i].gameplay_exclusive && GST_getGameState() != GAME_STATE_GAME)     
        {
            continue;
        }

        func_list[i].run();
    }
}
