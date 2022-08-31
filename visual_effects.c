#include "visual_effects.h"

// THE AMOUNT OF EXPLOSIONS TO POOL
#define EXPLOSION_POOL_COUNT 3

static int current_pooled_index = 0;

typedef struct
{
    Entity_t;
    bool is_rendered;
    u8 frame_ticker;
} VX_EnemyExplosion_t;

VX_EnemyExplosion_t *explosion_pool[EXPLOSION_POOL_COUNT];

void ResetExplosion(VX_EnemyExplosion_t *explosion)
{
    explosion->is_rendered = false;
    explosion->frame_ticker = 0;
    explosion->rect.height = 32;
    explosion->rect.width = 32;
    explosion->rect.x = DEACTIVATED_POSITION;
    explosion->rect.y = DEACTIVATED_POSITION;
    explosion->sprite = SPR_addSprite(&imgexplo, explosion->rect.x, explosion->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
}

VX_EnemyExplosion_t *CreateExplosion(void)
{
    VX_EnemyExplosion_t *createdExplosion = (VX_EnemyExplosion_t *)MEM_alloc(sizeof(VX_EnemyExplosion_t));
    ResetExplosion(createdExplosion);
    return createdExplosion;
}

void CreateExplosionPool(void)
{
    for (u8 i = 0; i < EXPLOSION_POOL_COUNT; i++)
    {
        explosion_pool[i] = CreateExplosion();
    }
}

void SpawnExposion(Vector2_t position)
{
    // TODO: Add spawn
    current_pooled_index ++; 
    if(current_pooled_index >= EXPLOSION_POOL_COUNT)
    {
        current_pooled_index = 0;    
    }
}