#include "enemy_core.h"

void ENY_runBulletSpawnSetup(Actor_t *a, s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    a->sprite->visibility = true;
    a->is_enabled = true;
    a->rect.x = x;
    a->rect.y = y;
    a->velocity.x = xSpeed;
    a->velocity.y = ySpeed;
}

void ENY_runSpawnSetup(ENY_Actor_t *a, s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    a->sprite->visibility = true;
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
    enyptr->sprite->visibility = false;
}

void ENY_reset(ENY_Actor_t *enyptr)
{
    enyptr->timeAlive = 0;
    enyptr->velocity.x = 0;
    enyptr->velocity.y = 0;

    enyptr->rect.x = DEACTIVATED_POSITION;
    enyptr->rect.y = DEACTIVATED_POSITION;

    enyptr->current_health = enyptr->initial_health;
    enyptr->is_enabled = false;
    enyptr->timeOfLastHit = 0;
    enyptr->sprite->visibility = false;
}

void ENY_kill(ENY_Actor_t *eny)
{
    spawnExposion(eny->rect);
    increaseScore(eny->worth);
    UI_updateScoreDisplay();
    eny->rect.y = DEACTIVATED_POSITION;
}

void ENY_handleHitByShot(ENY_Actor_t *eny)
{
    // Flash on hit

    if (PLY_is_player_shot_enabled())
    {

        eny->sprite->visibility = false;

        eny->timeOfLastHit = getLevelTime();

        resetShot();
        eny->current_health--;
        if (eny->current_health <= 0)
        {
            ENY_kill(eny);
        }
    }
}

void ENY_destroyEnemy(ENY_Actor_t *enyptr)
{
    SPR_releaseSprite(enyptr->sprite);
    MEM_free(enyptr);
}

void ENY_destroyBullet(Actor_t *enyptr)
{
    SPR_releaseSprite(enyptr->sprite);
    MEM_free(enyptr);
}
