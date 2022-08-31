#include "enemies.h"

void ResetBird(ENY_bird_t *birdptr, bool enabled)
{
    s32 game_time = getGameTime();
    birdptr->current_health = 10;
    birdptr->is_enabled = enabled;

    birdptr->rect.height = 32;
    birdptr->rect.width = 32;
    birdptr->rect.x = generateRandomNum(250, game_time);
    birdptr->rect.y = generateRandomNum(200, game_time) * -1;
    birdptr->worth = 10;
    birdptr->velocity.x = 1;
    birdptr->velocity.y = 2;    
}

ENY_bird_t *createBird(bool setAsEnabled)
{
    ENY_bird_t *result = NULL;
    result = (ENY_bird_t *)MEM_alloc(sizeof(ENY_bird_t));
 
    ResetBird(result, setAsEnabled);    

    result->sprite = SPR_addSprite(&bird, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));    
    
    return result;
}

void destroyBird(ENY_bird_t* birdptr)
{
    SPR_releaseSprite(birdptr->sprite);
    MEM_free(birdptr);
}