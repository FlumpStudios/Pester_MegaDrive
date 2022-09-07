#include "enemies.h"

// defines
#define MAX_BIRD_COUNT 2
// Structs
typedef struct
{
    Actor_t;
    bool is_enabled;
    u16 current_health;
    u16 worth;
} ENY_Actor;

typedef struct
{
    ENY_Actor;
} ENY_bird_t;

// fields
static ENY_bird_t *birdEnemies[MAX_BIRD_COUNT];

void resetBird(ENY_bird_t *birdptr, bool enabled)
{
    s32 game_time = getGameTime();
    birdptr->current_health = 4;
    birdptr->is_enabled = enabled;
    birdptr->rect.height = 32;
    birdptr->rect.width = 32;
    birdptr->rect.x = generateRandomNum(250, game_time);
    birdptr->rect.y = generateRandomNum(200, game_time) * -1;
    birdptr->worth = 10;
    birdptr->velocity.x = 1;
    birdptr->velocity.y = 2;
}

void ENY_reset(void)
{
    for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
    {
        resetBird(birdEnemies[i], TRUE);
    }
}

ENY_bird_t *createBird(bool setAsEnabled)
{
    ENY_bird_t *result = NULL;
    result = (ENY_bird_t *)MEM_alloc(sizeof(ENY_bird_t));

    resetBird(result, setAsEnabled);

    result->sprite = SPR_addSprite(&bird, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    return result;
}

void killBird(ENY_bird_t* eny)
{
    spawnExposion(eny->rect);
    resetBird(eny, true);
    increaseScore(10);
    updateScoreDisplay();    
}

void handleBirdHitByShot(ENY_bird_t* eny)
{
    // Flash on hit
    eny->sprite->visibility = false;
    resetShot();
    eny->current_health --;    
    if(eny->current_health <= 0)
    {
        killBird(eny);
    }
}

void updateBirdPosition(void)
{
    for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
    {
        if (birdEnemies[i]->rect.y > 0)
        {
            if (birdEnemies[i]->rect.x > getPlayerRect().x)
            {
                birdEnemies[i]->rect.x -= birdEnemies[i]->velocity.y;
            }
            else
            {
                birdEnemies[i]->rect.x += birdEnemies[i]->velocity.x;
            }
        }
        if (checkRectangleCollision(birdEnemies[i]->rect, getShotRect()))
        {
            handleBirdHitByShot(birdEnemies[i]);
        }
        else if (checkRectangleCollision(birdEnemies[i]->rect, getHitboxRect()))
        {
            killPlayer();
        }
        else
        {
            birdEnemies[i]->rect.y += birdEnemies[i]->velocity.y;
            birdEnemies[i]->sprite->visibility = true;
        }
        if (birdEnemies[i]->rect.y > 250)
        {
            resetBird(birdEnemies[i], true);
        }

        SPR_setPosition(birdEnemies[i]->sprite, birdEnemies[i]->rect.x, birdEnemies[i]->rect.y);
    };
}

void ENY_upate(void)
{
    updateBirdPosition();
}

void ENY_init(void)
{
    for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
    {
        birdEnemies[i] = createBird(true);
    }

    addTickFunc(ENY_upate, TRUE);
}

void destroyBird(ENY_bird_t *birdptr)
{
    SPR_releaseSprite(birdptr->sprite);
    MEM_free(birdptr);
}

void ENY_destruct(void)
{
    // Destory all enemies here
    for (u8 i = 0; i < MAX_BIRD_COUNT; i++)
    {
        destroyBird(birdEnemies[i]);
    }
}
