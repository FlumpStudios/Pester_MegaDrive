#include "enemies.h"

void ResetBird(ENY_bird_t *birdptr, bool enabled)
{
    s32 game_time = getGameTime();
    birdptr->base.current_health = 10;
    birdptr->base.is_enabled = enabled;

    birdptr->base.rect.height = 32;
    birdptr->base.rect.width = 32;
    birdptr->base.rect.x = generateRandomNum(250, game_time);
    birdptr->base.rect.y = generateRandomNum(200, game_time) * -1;
    birdptr->base.worth = 10;
    birdptr->base.velocity.x = 1;
    birdptr->base.velocity.y = 2;    
}

ENY_bird_t *createBird(bool setAsEnabled)
{
    ENY_bird_t *result = NULL;
    result = (ENY_bird_t *)MEM_alloc(sizeof(ENY_bird_t));
 
    result->base.sprite = SPR_addSprite(&bird, result->base.rect.x, result->base.rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));      
    
    ResetBird(result, setAsEnabled);    
    
    return result;
}

void destroyBird(ENY_bird_t* birdptr)
{
    SPR_releaseSprite(birdptr->base.sprite);
    MEM_free(birdptr);
}