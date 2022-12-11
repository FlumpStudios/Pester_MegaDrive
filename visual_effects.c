#include "visual_effects.h"
#include "game_update_observable.h"
// THE AMOUNT OF EXPLOSIONS TO POOL
#define EXPLOSION_POOL_COUNT 3

// THE NUMBER OF FRAMES THE EXPLOSIONS WILL STAY ON SCREEN FOR
#define EXPLOSION_SCREEN_TIME 10
#define BULLET_HIT_SCREEN_TIME 2

typedef struct
{
    Sprite *spriteSlot1;
    bool is_rendered;
    u8 frame_ticker;

} VX_effect_t;

VX_effect_t *explosion_pool[EXPLOSION_POOL_COUNT];
VX_effect_t *bulletHitEffect;

VX_effect_t *CreateExplosion(u8 i)
{
    VX_effect_t *createdExplosion = NULL;
    createdExplosion = MEM_alloc(sizeof(VX_effect_t));
    createdExplosion->is_rendered = false;
    createdExplosion->frame_ticker = 0;
    if (i == 0)
    {
        createdExplosion->spriteSlot1 = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    }
    else if (i == 1)
    {
        createdExplosion->spriteSlot1 = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    }
    else if (i == 2)
    {
        createdExplosion->spriteSlot1 = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL3, 0, FALSE, FALSE));
    }
    else
    {
        createdExplosion->spriteSlot1 = SPR_addSprite(&imgexplo, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    }
    return createdExplosion;
}

void createExplosionPool(void)
{
    for (u8 i = 0; i < EXPLOSION_POOL_COUNT; i++)
    {
        explosion_pool[i] = CreateExplosion(i);
    }
}

void VX_spawn_bullet_hit_effect(s16 x, s16 y)
{
    SPR_setPosition(bulletHitEffect->spriteSlot1, x, y);
    bulletHitEffect->is_rendered = true;
    bulletHitEffect->frame_ticker = 0;    
}

void VX_spawnExposion(Rectangle_t position)
{
    static int current_pooled_index = 0;

    if (current_pooled_index >= EXPLOSION_POOL_COUNT - 1)
    {
        current_pooled_index = 0;
    }

    explosion_pool[current_pooled_index]->is_rendered = true;

    SPR_setPosition(explosion_pool[current_pooled_index]->spriteSlot1, position.x, position.y);
    current_pooled_index++;
}

void VX_spawnExposionAtPosition(s16 x, s16 y)
{
    Rectangle_t pos = {x, y, 0, 0};
    VX_spawnExposion(pos);
}

static void VX_update(void)
{
    if(bulletHitEffect->is_rendered)
    {
        bulletHitEffect->frame_ticker ++;
        if(bulletHitEffect->frame_ticker > BULLET_HIT_SCREEN_TIME)
        {
            bulletHitEffect->is_rendered = false;
            SPR_setPosition(bulletHitEffect->spriteSlot1, DEACTIVATED_POSITION, DEACTIVATED_POSITION);
        }
    }
    

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
                SPR_setPosition(explosion_pool[i]->spriteSlot1, DEACTIVATED_POSITION, DEACTIVATED_POSITION);
            }
        }
    }
}



void VX_spawnLargeExplosion(s16 x, s16 y, u32 time, u8 interval)
{
    for (u8 i = 1; i < EXPLOSION_POOL_COUNT; i++)
    {
        if (time % (interval * i) == 0)
        {
            VX_spawnExposionAtPosition(x + CMN_generateRandomNum(36, time), y + CMN_generateRandomNum(36, time + 1));
        }
    }
}

void createBulletHit(void)
{
    bulletHitEffect = MEM_alloc(sizeof(VX_effect_t));
    bulletHitEffect->frame_ticker = 0;
    bulletHitEffect->spriteSlot1 = SPR_addSprite(&bulletHit, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 0, FALSE, FALSE));        
}

void VX_init(void)
{
    createExplosionPool();
    createBulletHit();    
    addTickFunc(VX_update, true);
}
