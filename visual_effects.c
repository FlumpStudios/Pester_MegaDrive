#include "visual_effects.h"
#include "game_update_observable.h"
// THE AMOUNT OF EXPLOSIONS TO POOL
#define EXPLOSION_POOL_COUNT 3

// THE NUMBER OF FRAMES THE EXPLOSIONS WILL STAY ON SCREEN FOR
#define EXPLOSION_SCREEN_TIME 10

typedef struct
{
    Sprite *sprite;
    bool is_rendered;
    u8 frame_ticker;

} VX_EnemyExplosion_t;

VX_EnemyExplosion_t *explosion_pool[EXPLOSION_POOL_COUNT];

VX_EnemyExplosion_t *CreateExplosion(u8 i)
{
    VX_EnemyExplosion_t *createdExplosion  = NULL;
    createdExplosion = MEM_alloc(sizeof(VX_EnemyExplosion_t));
    createdExplosion->is_rendered = false;
    createdExplosion->frame_ticker = 0;
    if(i == 0)
    {
        createdExplosion->sprite = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    }
    else if(i == 1)
    {
        createdExplosion->sprite = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    }
    else if(i == 2)
    {
        createdExplosion->sprite = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL3, 0, FALSE, FALSE));
    }
    else 
    {
        createdExplosion->sprite = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    }
    return createdExplosion;
}

void CreateExplosionPool(void)
{
    for (u8 i = 0; i < EXPLOSION_POOL_COUNT; i++)
    {
        explosion_pool[i] = CreateExplosion(i);
    }
}

void spawnExposion(Rectangle_t position)
{
    static int current_pooled_index = 0;

    if (current_pooled_index >= EXPLOSION_POOL_COUNT - 1)
    {
        current_pooled_index = 0;
    }

    explosion_pool[current_pooled_index]->is_rendered = true;
    
    SPR_setPosition(explosion_pool[current_pooled_index]->sprite, position.x, position.y);
    current_pooled_index++;
}

static void VX_update(void)
{
    for (u8 i = 0; i < EXPLOSION_POOL_COUNT; i++)
    {
        if (explosion_pool[i]->is_rendered)
        {
            explosion_pool[i]->frame_ticker++;
            if (explosion_pool[i]->frame_ticker >= EXPLOSION_SCREEN_TIME)
            {
                explosion_pool[i]->is_rendered = false;
                explosion_pool[i]->frame_ticker = 0;
                // NOTE: Sprite visibility was being weird, so just hiding off screen for now.
                SPR_setPosition(explosion_pool[i]->sprite, DEACTIVATED_POSITION, DEACTIVATED_POSITION);
            }
        }
    }
}

void VX_init(void)
{
    CreateExplosionPool();
    addTickFunc(VX_update, true);
}
