#include"game_update_observable.h"

// HACK: Should be a dynamic array, but had all kinds of problems when I tried implementing. So using static array for now
// TODO: Make dynamic
#define FUNC_LIST_CAPACITY 10

typedef struct { 
 tickFunction Run;
} Func_t;

static Func_t func_list[FUNC_LIST_CAPACITY];
static u16 cur_index = 0;

void AddFunc(tickFunction f)
{
    if(cur_index < FUNC_LIST_CAPACITY)
    {
        func_list[cur_index].Run = f;
        cur_index++;
    }
}

void RunStoredTickFunctions(void)
{    
    for (u16 i = 0; i < cur_index; i++)
    {
        func_list[i].Run();
    }
}
