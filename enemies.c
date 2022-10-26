#include "enemies.h"

// defines
#define BIRD_POOL_COUNT 5
#define GRABBER_POOL_SIZE 5

// Structs
typedef struct
{
    Actor_t;
    Range_t x_speed_range;
    Range_t y_speed_range;
    bool is_enabled;
    u16 initial_health;
    u16 current_health;
    u16 worth;
} ENY_Actor;

// fields
static ENY_Actor *birdEnemies[BIRD_POOL_COUNT];
static ENY_Actor *grabberEnemies[GRABBER_POOL_SIZE];

static int bird_count = 0;
static int grabber_count = 0;

static int bird_active_count = 0;
static int grabber_active_count = 0;

static int bird_spawn_frequency = 50;
static int grabber_spawn_frequency = 100;

static int bird_current_pool_index = 0;
static int grabber_current_pool_index = 0;

static int bird_spawn_ticker = 0;
static int grabber_spawn_ticker = 0;

void ENY_setBirdCount(int count)
{
    bird_count = count;
}

void ENY_setGrabberCount(int count)
{
    grabber_count = count;
}

void resetEnemy(ENY_Actor *enyptr)
{
    s32 game_time = getGameTime();
    enyptr->current_health = enyptr->initial_health;
    enyptr->is_enabled = false;
    enyptr->rect.y = generateRandomNumWithRange(50, 100, game_time) * -1;
    enyptr->rect.x = generateRandomNumWithRange(20, 300, game_time);
    enyptr->velocity.x = generateRandomNumWithRange(enyptr->x_speed_range.min, enyptr->x_speed_range.max, game_time);
    enyptr->velocity.y = generateRandomNumWithRange(enyptr->y_speed_range.min, enyptr->y_speed_range.max, game_time);
}

void ENY_reset(void)
{
    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        resetEnemy(birdEnemies[i]);
    }

    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        resetEnemy(grabberEnemies[i]);
    }

    bird_count = 0;
    grabber_count = 0;

    bird_active_count = 0;
    grabber_active_count = 0;

    bird_spawn_frequency = 50;
    grabber_spawn_frequency = 100;

    bird_current_pool_index = 0;
    grabber_current_pool_index = 0;

    bird_spawn_ticker = 0;
    grabber_spawn_ticker = 0;
}

ENY_Actor *createBird()
{

    ENY_Actor *result = NULL;
    result = (ENY_Actor *)MEM_alloc(sizeof(ENY_Actor));

    result->initial_health = 3;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 10;
    result->x_speed_range.min = 1;
    result->x_speed_range.max = 1;
    result->y_speed_range.min = 1;
    result->y_speed_range.max = 3;

    resetEnemy(result);
    result->sprite = SPR_addSprite(&bird, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

ENY_Actor *createGrabber()
{
    ENY_Actor *result = NULL;
    result = (ENY_Actor *)MEM_alloc(sizeof(ENY_Actor));

    result->initial_health = 1;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 5;
    result->x_speed_range.min = 0;
    result->x_speed_range.max = 0;
    result->y_speed_range.min = 1;
    result->y_speed_range.max = 5;

    resetEnemy(result);

    result->sprite = SPR_addSprite(&grabber, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

void killEnemy(ENY_Actor *eny)
{
    spawnExposion(eny->rect);
    increaseScore(eny->worth);
    updateScoreDisplay();
    eny->rect.y = DEACTIVATED_POSITION;
}

void handleEnemyHitByShot(ENY_Actor *eny)
{
    // Flash on hit
    eny->sprite->visibility = false;

    resetShot();
    eny->current_health--;
    if (eny->current_health <= 0)
    {
        killEnemy(eny);
    }
}

void updateBird(void)
{
    if (bird_count > 0)
    {
        bird_spawn_ticker++;
        if (bird_spawn_ticker > bird_spawn_frequency && bird_active_count < bird_count)
        {
            birdEnemies[bird_current_pool_index]->is_enabled = true;
            bird_active_count++;
            bird_current_pool_index++;
            if (bird_current_pool_index >= BIRD_POOL_COUNT)
            {
                bird_current_pool_index = 0;
            }
            bird_spawn_ticker = 0;
        }
    }

    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        if (birdEnemies[i]->is_enabled)
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
                handleEnemyHitByShot(birdEnemies[i]);
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
                bird_active_count--;
                resetEnemy(birdEnemies[i]);
            }
        }
        SPR_setPosition(birdEnemies[i]->sprite, birdEnemies[i]->rect.x, birdEnemies[i]->rect.y);
    };
}

void updateGrabber(void)
{
    if (grabber_count > 0)
    {
        grabber_spawn_ticker++;
        if (grabber_spawn_ticker > grabber_spawn_frequency && grabber_active_count < grabber_count)
        {
            grabberEnemies[grabber_current_pool_index]->is_enabled = true;
            grabber_active_count++;

            grabber_current_pool_index++;
            if (grabber_current_pool_index >= GRABBER_POOL_SIZE)
            {
                grabber_current_pool_index = 0;
            }
            grabber_spawn_ticker = 0;
        }
    }

    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        if (grabberEnemies[i]->is_enabled)
        {
            if (checkRectangleCollision(grabberEnemies[i]->rect, getShotRect()))
            {
                handleEnemyHitByShot(grabberEnemies[i]);
            }
            else if (checkRectangleCollision(grabberEnemies[i]->rect, getHitboxRect()))
            {
                killPlayer();
            }
            else
            {
                grabberEnemies[i]->rect.y += grabberEnemies[i]->velocity.y;
                grabberEnemies[i]->sprite->visibility = true;
            }
            if (grabberEnemies[i]->rect.y > 250)
            {
                grabber_active_count--;
                resetEnemy(grabberEnemies[i]);
            }
        }
        SPR_setPosition(grabberEnemies[i]->sprite, grabberEnemies[i]->rect.x, grabberEnemies[i]->rect.y);
    };
}

void ENY_upate(void)
{
    updateBird();
    updateGrabber();
}

void ENY_init(void)
{
    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        birdEnemies[i] = createBird();
    }

    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        grabberEnemies[i] = createGrabber();
    }

    addTickFunc(ENY_upate, TRUE);
}

void destroyEnemy(ENY_Actor *birdptr)
{
    SPR_releaseSprite(birdptr->sprite);
    MEM_free(birdptr);
}
void ENY_destruct(void)
{
    // Destory all enemies here
    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        destroyEnemy(birdEnemies[i]);
    }
    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        destroyEnemy(grabberEnemies[i]);
    }
}
