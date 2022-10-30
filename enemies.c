#include "enemies.h"

// defines
#define BIRD_POOL_COUNT 5
#define GRABBER_POOL_SIZE 5

// Structs

// fields
static ENY_Actor *birdEnemies[BIRD_POOL_COUNT];
static ENY_Actor *grabberEnemies[GRABBER_POOL_SIZE];

static int bird_active_count = 0;
static int grabber_active_count = 0;

static int bird_current_pool_index = 0;
static int grabber_current_pool_index = 0;

void ENY_spawnBird(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    if (bird_active_count <= BIRD_POOL_COUNT)
    {
        bird_active_count++;
        ENY_runSpawnSetup(birdEnemies[bird_current_pool_index],x,y,xSpeed, ySpeed);
        bird_current_pool_index++;
        if (bird_current_pool_index > BIRD_POOL_COUNT)
        {
            bird_current_pool_index = 0;
        }
    }
}

void ENY_spawnGrabber(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    if (grabber_active_count <= GRABBER_POOL_SIZE)
    {
        grabber_active_count++;
        ENY_runSpawnSetup(grabberEnemies[grabber_current_pool_index],x,y,xSpeed, ySpeed);
        grabber_current_pool_index++;
        if (grabber_current_pool_index > GRABBER_POOL_SIZE)
        {
            grabber_current_pool_index = 0;
        }
    }
}

void ENY_resetAllEnemies(void)
{
    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        ENY_reset(birdEnemies[i]);
    }

    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        ENY_reset(grabberEnemies[i]);
    }

    bird_active_count = 0;
    grabber_active_count = 0;

    bird_current_pool_index = 0;
    grabber_current_pool_index = 0;
}

static ENY_Actor *createBird()
{
    ENY_Actor *result = NULL;
    result = (ENY_Actor *)MEM_alloc(sizeof(ENY_Actor));

    result->initial_health = 3;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 10;

    ENY_reset(result);
    result->sprite = SPR_addSprite(&bird, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static ENY_Actor *createGrabber()
{
    ENY_Actor *result = NULL;
    result = (ENY_Actor *)MEM_alloc(sizeof(ENY_Actor));

    result->initial_health = 1;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 5;
    result->timeAlive = 0;
    result->lifeTime = 300;
    ENY_reset(result);

    result->sprite = SPR_addSprite(&grabber, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static void updateBird(void)
{
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
                ENY_handleHitByShot(birdEnemies[i]);
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
                ENY_reset(birdEnemies[i]);
            }
        }
        SPR_setPosition(birdEnemies[i]->sprite, birdEnemies[i]->rect.x, birdEnemies[i]->rect.y);
    };
}

static void updateGrabber(void)
{
    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        if (grabberEnemies[i]->is_enabled)
        {
            grabberEnemies[i]->timeAlive ++;            
            if (checkRectangleCollision(grabberEnemies[i]->rect, getShotRect()))
            {
                ENY_handleHitByShot(grabberEnemies[i]);
            }
            else if (checkRectangleCollision(grabberEnemies[i]->rect, getHitboxRect()))
            {
                killPlayer();
            }
            else
            {
                grabberEnemies[i]->rect.y += grabberEnemies[i]->velocity.y;
                grabberEnemies[i]->rect.x += grabberEnemies[i]->velocity.x;
                
                grabberEnemies[i]->sprite->visibility = true;
            }
            if (grabberEnemies[i]->rect.y > 250 
                || grabberEnemies[i]->timeAlive > grabberEnemies[i]->lifeTime
                || grabberEnemies[i]->rect.x > 440
                || grabberEnemies[i]->rect.x < -128)
            {
                grabber_active_count--;
                ENY_reset(grabberEnemies[i]);
            }
        }
        SPR_setPosition(grabberEnemies[i]->sprite, grabberEnemies[i]->rect.x, grabberEnemies[i]->rect.y);
    };
}

void update(void)
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

    addTickFunc(update, TRUE);
}


void ENY_destruct(void)
{
    // Destory all enemies here
    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        ENY_destroyEnemy(birdEnemies[i]);
    }
    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        ENY_destroyEnemy(grabberEnemies[i]);
    }
}
