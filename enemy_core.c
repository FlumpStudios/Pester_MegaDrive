#include "enemy_core.h"

void ENY_runSpawnSetup(ENY_Actor *a, s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    a->is_enabled = true;
    a->rect.x = x;
    a->rect.y = y;
    a->velocity.x = xSpeed;
    a->velocity.y = ySpeed;
}

void ENY_reset(ENY_Actor *enyptr)
{
    enyptr->timeAlive = 0;
    enyptr->velocity.x = 0;
    enyptr->velocity.y = 0;

    enyptr->rect.x = DEACTIVATED_POSITION;
    enyptr->rect.y = DEACTIVATED_POSITION;

    enyptr->current_health = enyptr->initial_health;
    enyptr->is_enabled = false;
}

void ENY_kill(ENY_Actor *eny)
{
    spawnExposion(eny->rect);
    increaseScore(eny->worth);
    UI_updateScoreDisplay();
    eny->rect.y = DEACTIVATED_POSITION;
}

void ENY_handleHitByShot(ENY_Actor *eny)
{
    // Flash on hit
    eny->sprite->visibility = false;

    resetShot();
    eny->current_health--;
    if (eny->current_health <= 0)
    {
        ENY_kill(eny);
    }
}

void ENY_destroyEnemy(ENY_Actor *enyptr)
{
    SPR_releaseSprite(enyptr->sprite);
    MEM_free(enyptr);
}
