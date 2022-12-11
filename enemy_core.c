#include "enemy_core.h"
#include "audio.h"

void ENY_runBulletSpawnSetup(Actor_t *a, s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    a->spriteSlot1->visibility = true;
    if (a->spriteSlot2 != NULL)
    {
        a->spriteSlot2->visibility = true;
    }
    a->is_enabled = true;
    a->rect.x = x;
    a->rect.y = y;
    a->velocity.x = xSpeed;
    a->velocity.y = ySpeed;
}

void ENY_runSpawnSetup(ENY_Actor_t *a, s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    a->spriteSlot1->visibility = true;
    if (a->spriteSlot2 != NULL)
    {
        a->spriteSlot2->visibility = true;
    }
    a->is_enabled = true;
    a->rect.x = x;
    a->rect.y = y;
    a->velocity.x = xSpeed;
    a->velocity.y = ySpeed;
    a->variationId = 0;
}

void ENY_reset_bullet(Actor_t *enyptr)
{
    enyptr->velocity.x = 0;
    enyptr->velocity.y = 0;

    enyptr->rect.x = DEACTIVATED_POSITION;
    enyptr->rect.y = DEACTIVATED_POSITION;
    enyptr->is_enabled = false;
    enyptr->spriteSlot1->visibility = false;
    if (enyptr->spriteSlot2 != NULL)
    {
        enyptr->spriteSlot2->visibility = false;
    }
}

static void ENY_initialise(ENY_Actor_t *e)
{
    e->spriteSlot1 = NULL;
    e->spriteSlot2 = NULL;

    e->rect.x = 0;
    e->rect.y = 0;
    e->rect.width = 0;
    e->rect.height = 0;

    e->velocity.x = 0;
    e->velocity.y = 0;

    e->is_enabled = false;
    e->current_health = 0;
    e->initial_health = 0;
    e->lifeTime = 0;
    e->speed = 0;
    e->timeAlive = 0;
    e->timeOfLastHit = 0;
    e->variationId = 0;
    e->worth = 0;
}

ENY_Actor_t *ENY_new_enemy_actor(void)
{
    ENY_Actor_t *result = MEM_alloc(sizeof(ENY_Actor_t));
    ENY_initialise(result);
    return result;
}

void ENY_reset(ENY_Actor_t *enyptr)
{
    enyptr->timeAlive = 0;
    enyptr->rect.x = DEACTIVATED_POSITION;
    enyptr->rect.y = DEACTIVATED_POSITION;
    enyptr->current_health = enyptr->initial_health;
    enyptr->is_enabled = false;
    enyptr->timeOfLastHit = 0;
    enyptr->velocity.x = 0;
    enyptr->velocity.y = 0;

    enyptr->spriteSlot1->visibility = false;
    if (enyptr->spriteSlot2 != NULL)
    {
        enyptr->spriteSlot2->visibility = false;
    }
}

void ENY_kill(ENY_Actor_t *eny)
{
    GST_increaseChain(1);
    AUD_play_explosion();
    VX_spawnExposion(eny->rect);
    GST_increaseScore(eny->worth);
    UI_updateChainDisplay();
    UI_updateScoreDisplay();
    eny->rect.y = DEACTIVATED_POSITION;
}

void ENY_handleHitByShot(ENY_Actor_t *eny)
{
    // Flash on hit
    if (PLY_is_player_shot_enabled())
    {
        eny->spriteSlot1->visibility = false;

        if (eny->spriteSlot2 != NULL)
        {
            eny->spriteSlot2->visibility = false;
        }

        eny->timeOfLastHit = GST_getLevelTime();

        eny->current_health--;
        if (eny->current_health <= 0)
        {
            ENY_kill(eny);
        }
        else
        {
            Rectangle_t playerLocation = PLY_getShotRect();
            VX_spawn_bullet_hit_effect(playerLocation.x, playerLocation.y);
        }
        PLY_resetShot();
    }
}

void ENY_destroyEnemy(ENY_Actor_t *enyptr)
{
    if (enyptr != NULL)
    {
        SPR_releaseSprite(enyptr->spriteSlot1);
        if (enyptr->spriteSlot1 != NULL)
        {
            SPR_releaseSprite(enyptr->spriteSlot2);
        }
        MEM_free(enyptr);
        enyptr = NULL;
    }
}

void ENY_destroyBullet(Actor_t *enyptr)
{
    if (enyptr != NULL)
    {
        SPR_releaseSprite(enyptr->spriteSlot1);
        if (enyptr->spriteSlot1 != NULL)
        {
            SPR_releaseSprite(enyptr->spriteSlot2);
        }
        MEM_free(enyptr);
        enyptr = NULL;
    }
}
