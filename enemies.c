#include "enemies.h"

// defines
#define FLOATER_POOL_COUNT 6
#define POPCORN_POOL_COUNT 10

#define BOUNCER_POOL_COUNT 2
#define BIRD_POOL_COUNT 5
#define GRABBER_POOL_SIZE 6
#define ASTROID_POOL_SIZE 6
#define CIRCLE_BULLETS_POOL_SIZE 10
#define ENEMY_HIT_FLASH_TIME 2
#define ROCKET_BULLET_POOL_SIZE 5

#define GRABBER_ROCKET_SPAWN_DELAY 50
#define BOUNCER_BULLET_SPAWN_DELAY 150
#define FLOATER_BULLET_SPAWN_DELAY 75

// fields
static ENY_Actor_t *birdEnemies[BIRD_POOL_COUNT];
static ENY_Actor_t *grabberEnemies[GRABBER_POOL_SIZE];
static ENY_Actor_t *astroidEnemies[ASTROID_POOL_SIZE];
static ENY_Actor_t *bouncerEnemies[BOUNCER_POOL_COUNT];
static ENY_Actor_t *floaterEnemies[FLOATER_POOL_COUNT];
static ENY_Actor_t *popcornEnemies[POPCORN_POOL_COUNT];

static Actor_t *circleBullets[CIRCLE_BULLETS_POOL_SIZE];
static Actor_t *rocketBullets[ROCKET_BULLET_POOL_SIZE];

static int bird_active_count = 0;
static int grabber_active_count = 0;
static int astroid_active_count = 0;
static int circle_bullet_active_count = 0;
static int rocket_bullet_active_count = 0;
static int bouncer_active_count = 0;
static int floater_active_count = 0;
static int popcorn_active_count = 0;

static int bird_current_pool_index = 0;
static int grabber_current_pool_index = 0;
static int astroid_current_pool_index = 0;
static int circle_bullet_current_pool_index = 0;
static int rocket_bullet_current_pool_index = 0;
static int bouncer_current_pool_index = 0;
static int floater_current_pool_index = 0;
static int popcorn_current_pool_index = 0;

void ENY_spawnRocketBullet(s16 x, s16 y, s16 ySpeed)
{
    ENY_runBulletSpawnSetup(rocketBullets[rocket_bullet_current_pool_index], x, y, 0, ySpeed);
    rocket_bullet_active_count++;
    rocket_bullet_current_pool_index++;
    if (rocket_bullet_current_pool_index >= ROCKET_BULLET_POOL_SIZE)
    {
        rocket_bullet_current_pool_index = 0;
    }
}

void ENY_spawncircleBullet(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    ENY_runBulletSpawnSetup(circleBullets[circle_bullet_current_pool_index], x, y, xSpeed, ySpeed);
    circle_bullet_active_count++;
    circle_bullet_current_pool_index++;
    if (circle_bullet_current_pool_index >= CIRCLE_BULLETS_POOL_SIZE)
    {
        circle_bullet_current_pool_index = 0;
    }
}

void ENY_spawncircleBullets_squarepattern(s16 x, s16 y)
{
    ENY_spawncircleBullet(x, y, -1, 0);
    ENY_spawncircleBullet(x, y, 0, 1);
    ENY_spawncircleBullet(x, y, 1, 0);
    ENY_spawncircleBullet(x, y, 0, -1);
}

void ENY_spawncircleBullets_tripplepattern(s16 x, s16 y)
{
    ENY_spawncircleBullet(x, y, -1, 1);
    ENY_spawncircleBullet(x, y, 0, 1);
    ENY_spawncircleBullet(x, y, 1, 1);
}

void ENY_spawncircleBullets_forkedpattern(s16 x, s16 y)
{
    ENY_spawncircleBullet(x, y, -1, 2);
    ENY_spawncircleBullet(x, y, 1, 2);
}

void ENY_spawncircleBullets_sidepattern(s16 x, s16 y, s8 speed)
{
    ENY_spawncircleBullet(x, y, speed * -1, 0);
    ENY_spawncircleBullet(x, y, speed, 0);
}

void ENY_spawnBouncer(s16 x, s16 y, s16 xSpeed, s16 ySpeed, u16 lifeTime)
{
    if (bouncer_active_count < BOUNCER_POOL_COUNT)
    {
        bouncer_active_count++;
        ENY_runSpawnSetup(bouncerEnemies[bouncer_current_pool_index], x, y, xSpeed, ySpeed);
        bouncerEnemies[bouncer_current_pool_index]->lifeTime = lifeTime;
        bouncer_current_pool_index++;
    }

    if (bouncer_current_pool_index >= BOUNCER_POOL_COUNT)
    {
        bouncer_current_pool_index = 0;
    }
}

void ENY_spawnPopcorn(s16 x, s16 y, s16 xSpeed, s16 ySpeed, u8 variation)
{
    if (popcorn_active_count < POPCORN_POOL_COUNT)
    {
        popcorn_active_count++;
        ENY_runSpawnSetup(popcornEnemies[popcorn_current_pool_index], x, y, xSpeed, ySpeed);
        popcornEnemies[popcorn_current_pool_index]->variationId = variation;
        SPR_setAnim(popcornEnemies[popcorn_current_pool_index]->sprite, variation);

        popcorn_current_pool_index++;
    }

    if (popcorn_current_pool_index >= POPCORN_POOL_COUNT)
    {
        popcorn_current_pool_index = 0;
    }
}

void ENY_spawnFloater(s16 x, s16 y, s16 ySpeed, u8 variation)
{
    if (floater_active_count < FLOATER_POOL_COUNT)
    {
        floater_active_count++;
        ENY_runSpawnSetup(floaterEnemies[floater_current_pool_index], x, y, 0, ySpeed);
        floaterEnemies[floater_current_pool_index]->variationId = variation;

        SPR_setAnim(floaterEnemies[floater_current_pool_index]->sprite, variation);

        floater_current_pool_index++;
    }

    if (floater_current_pool_index >= FLOATER_POOL_COUNT)
    {
        floater_current_pool_index = 0;
    }
}

void ENY_spawnBird(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    if (bird_active_count < BIRD_POOL_COUNT)
    {
        bird_active_count++;
        ENY_runSpawnSetup(birdEnemies[bird_current_pool_index], x, y, xSpeed, ySpeed);
        bird_current_pool_index++;
    }

    if (bird_current_pool_index >= BIRD_POOL_COUNT)
    {
        bird_current_pool_index = 0;
    }
}

void ENY_spawnGrabber(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    if (grabber_active_count < GRABBER_POOL_SIZE)
    {
        grabber_active_count++;
        ENY_runSpawnSetup(grabberEnemies[grabber_current_pool_index], x, y, xSpeed, ySpeed);
        grabber_current_pool_index++;
    }

    if (grabber_current_pool_index >= GRABBER_POOL_SIZE)
    {
        grabber_current_pool_index = 0;
    }
}

void ENY_spawnAstroid(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    if (astroid_active_count < ASTROID_POOL_SIZE)
    {
        astroid_active_count++;
        ENY_runSpawnSetup(astroidEnemies[astroid_current_pool_index], x, y, xSpeed, ySpeed);
        astroid_current_pool_index++;
    }

    if (astroid_current_pool_index >= ASTROID_POOL_SIZE)
    {
        astroid_current_pool_index = 0;
    }
}

void ENY_resetAllBullets(void)
{
    for (u8 i = 0; i < CIRCLE_BULLETS_POOL_SIZE; i++)
    {
        ENY_reset_bullet(circleBullets[i]);
    }

    for (u8 i = 0; i < ROCKET_BULLET_POOL_SIZE; i++)
    {
        ENY_reset_bullet(rocketBullets[i]);
    }
}

void ENY_resetAllEnemies(void)
{
    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        ENY_reset(birdEnemies[i]);
        SPR_setPosition(birdEnemies[i]->sprite, birdEnemies[i]->rect.x, birdEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        ENY_reset(grabberEnemies[i]);
        SPR_setPosition(grabberEnemies[i]->sprite, grabberEnemies[i]->rect.x, grabberEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < ASTROID_POOL_SIZE; i++)
    {
        ENY_reset(astroidEnemies[i]);
        SPR_setPosition(astroidEnemies[i]->sprite, astroidEnemies[i]->rect.x, astroidEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < BOUNCER_POOL_COUNT; i++)
    {
        ENY_reset(bouncerEnemies[i]);
        SPR_setPosition(bouncerEnemies[i]->sprite, bouncerEnemies[i]->rect.x, bouncerEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < FLOATER_POOL_COUNT; i++)
    {
        ENY_reset(floaterEnemies[i]);
        SPR_setPosition(floaterEnemies[i]->sprite, floaterEnemies[i]->rect.x, floaterEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < POPCORN_POOL_COUNT; i++)
    {
        ENY_reset(popcornEnemies[i]);
        SPR_setPosition(popcornEnemies[i]->sprite, popcornEnemies[i]->rect.x, popcornEnemies[i]->rect.y);
    }

    bird_active_count = 0;
    grabber_active_count = 0;
    astroid_active_count = 0;
    bouncer_active_count = 0;
    floater_active_count = 0;
    popcorn_active_count = 0;

    bird_current_pool_index = 0;
    grabber_current_pool_index = 0;
    astroid_current_pool_index = 0;
    bouncer_current_pool_index = 0;
    floater_current_pool_index = 0;
    popcorn_current_pool_index = 0;
}

static ENY_Actor_t *createPopcorn(void)
{
    ENY_Actor_t *result = NULL;
    result = (ENY_Actor_t *)MEM_alloc(sizeof(ENY_Actor_t));

    result->initial_health = 1;
    result->rect.height = 16;
    result->rect.width = 16;
    result->worth = 5;
    result->timeOfLastHit = 0;
    result->lifeTime = 0;

    ENY_reset(result);
    result->sprite = SPR_addSprite(&popcorn, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    return result;
}

static ENY_Actor_t *createFloater(void)
{
    ENY_Actor_t *result = NULL;
    result = (ENY_Actor_t *)MEM_alloc(sizeof(ENY_Actor_t));

    result->initial_health = 2;
    result->rect.height = 16;
    result->rect.width = 32;
    result->worth = 25;
    result->timeOfLastHit = 0;
    result->lifeTime = 0;

    ENY_reset(result);
    result->sprite = SPR_addSprite(&floater, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static ENY_Actor_t *createBouncer(void)
{
    ENY_Actor_t *result = NULL;
    result = (ENY_Actor_t *)MEM_alloc(sizeof(ENY_Actor_t));

    result->initial_health = 4;
    result->rect.height = 16;
    result->rect.width = 32;
    result->worth = 50;
    result->timeOfLastHit = 0;
    result->lifeTime = 0;

    ENY_reset(result);
    result->sprite = SPR_addSprite(&bouncer, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static ENY_Actor_t *createBird(void)
{
    ENY_Actor_t *result = NULL;
    result = (ENY_Actor_t *)MEM_alloc(sizeof(ENY_Actor_t));

    result->initial_health = 3;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 10;
    result->timeOfLastHit = 0;

    ENY_reset(result);
    result->sprite = SPR_addSprite(&bird, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static ENY_Actor_t *createGrabber(void)
{
    ENY_Actor_t *result = NULL;
    result = (ENY_Actor_t *)MEM_alloc(sizeof(ENY_Actor_t));

    result->initial_health = 1;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 5;
    result->timeAlive = 0;
    result->lifeTime = 0;
    result->timeOfLastHit = 0;
    ENY_reset(result);

    result->sprite = SPR_addSprite(&grabber, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static ENY_Actor_t *createAstroid(void)
{
    ENY_Actor_t *result = NULL;
    result = (ENY_Actor_t *)MEM_alloc(sizeof(ENY_Actor_t));

    result->initial_health = 1;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 5;
    result->timeOfLastHit = 0;
    ENY_reset(result);

    result->sprite = SPR_addSprite(&astroid, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static Actor_t *CreateCircleBullet(void)
{
    Actor_t *result = NULL;
    result = (Actor_t *)MEM_alloc(sizeof(Actor_t));

    result->rect.height = 16;
    result->rect.width = 16;
    ENY_reset_bullet(result);

    result->sprite = SPR_addSprite(&enemyBullet, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static Actor_t *CreateRocketBullet(void)
{
    Actor_t *result = NULL;
    result = (Actor_t *)MEM_alloc(sizeof(Actor_t));

    result->rect.height = 8;
    result->rect.width = 16;
    ENY_reset_bullet(result);

    result->sprite = SPR_addSprite(&enemyRocket, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    return result;
}

static void updateFloater(void)
{
    if (floater_active_count > 0)
    {
        for (u8 i = 0; i < FLOATER_POOL_COUNT; i++)
        {
            ENY_Actor_t *enemy = floaterEnemies[i];
            if (enemy->is_enabled)
            {
                enemy->timeAlive++;

                enemy->rect.y += enemy->velocity.y;

                // IF on screen
                if (enemy->rect.y > 0)
                {
                    if (enemy->timeAlive % FLOATER_BULLET_SPAWN_DELAY == 0)
                    {
                        if (enemy->variationId == 0 && enemy->rect.y < 100)
                        {
                            ENY_spawncircleBullets_forkedpattern(enemy->rect.x + 8, enemy->rect.y + 3);
                        }
                        else if (enemy->variationId == 1)
                        {
                            ENY_spawncircleBullets_sidepattern(enemy->rect.x + 8, enemy->rect.y + 3, 2);
                        }
                    }
                }
                if (checkRectangleCollision(enemy->rect, getShotRect()))
                {
                    ENY_handleHitByShot(enemy);
                }
                else if (checkRectangleCollision(enemy->rect, getHitboxRect()))
                {
                    runPlayerHit();
                }

                if (enemy->timeOfLastHit > 0 && getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
                {
                    enemy->sprite->visibility = true;
                }

                if (enemy->rect.y > 250 || enemy->rect.x > 440 || enemy->rect.x < -128)
                {
                    floater_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->sprite, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updatePopcorn(void)
{
    if (popcorn_active_count > 0)
    {
        for (u8 i = 0; i < POPCORN_POOL_COUNT; i++)
        {
            ENY_Actor_t *enemy = popcornEnemies[i];
            if (enemy->is_enabled)
            {
                enemy->timeAlive++;

                // Custom behavior for variations 2+
                if (enemy->variationId >= 2)
                {
                    if (enemy->timeAlive % 2 == 0)
                    {
                        if (enemy->variationId == 2)
                        {
                            if (enemy->timeAlive % 80 < 40)
                            {
                                enemy->rect.x += enemy->velocity.x;
                            }
                            else
                            {
                                enemy->rect.x -= enemy->velocity.x;
                            }
                            enemy->rect.y += enemy->velocity.y;
                        }

                        if (enemy->variationId == 3)
                        {
                            if (enemy->timeAlive % 20 < 10)
                            {
                                enemy->rect.x += enemy->velocity.x;
                            }
                            else
                            {
                                enemy->rect.x -= enemy->velocity.x;
                            }
                            enemy->rect.y += enemy->velocity.y;
                        }

                        // For side entering snakes
                        if (enemy->variationId == 4 || enemy->variationId == 5)
                        {
                            if (enemy->timeAlive % 20 < 10)
                            {
                                enemy->rect.y += enemy->velocity.y;
                            }
                            else
                            {
                                enemy->rect.y -= enemy->velocity.y;
                            }
                            
                            enemy->rect.x += enemy->velocity.x;
                        }
                    }
                }
                else
                {
                    enemy->rect.x += enemy->velocity.x;
                    enemy->rect.y += enemy->velocity.y;
                }

                if (checkRectangleCollision(enemy->rect, getShotRect()))
                {
                    ENY_handleHitByShot(enemy);
                }
                else if (checkRectangleCollision(enemy->rect, getHitboxRect()))
                {
                    runPlayerHit();
                }

                if (enemy->timeOfLastHit > 0 && getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
                {
                    enemy->sprite->visibility = true;
                }

                if (enemy->rect.y > 250 || enemy->rect.y < -100 || enemy->rect.x > 440 || enemy->rect.x < -128)
                {
                    popcorn_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->sprite, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updateBouncer(void)
{
    if (bouncer_active_count > 0)
    {
        for (u8 i = 0; i < BOUNCER_POOL_COUNT; i++)
        {
            ENY_Actor_t *enemy = bouncerEnemies[i];
            if (enemy->is_enabled)
            {
                enemy->timeAlive++;

                if (enemy->timeAlive < 90)
                {
                    // Slide into screen from top
                    enemy->rect.y += enemy->velocity.y;
                }

                if (enemy->timeAlive > enemy->lifeTime)
                {
                    if (enemy->rect.x < 150)
                    {
                        enemy->rect.x -= enemy->velocity.x;
                    }
                    else
                    {
                        enemy->rect.x += enemy->velocity.x;
                    }
                }

                // IF on screen
                if (enemy->rect.y > 0)
                {
                    if (enemy->timeAlive % BOUNCER_BULLET_SPAWN_DELAY == 0)
                    {
                        ENY_spawncircleBullets_tripplepattern(enemy->rect.x + 8, enemy->rect.y + 3);
                    }
                }
                if (checkRectangleCollision(enemy->rect, getShotRect()))
                {
                    ENY_handleHitByShot(enemy);
                }
                else if (checkRectangleCollision(enemy->rect, getHitboxRect()))
                {
                    runPlayerHit();
                }

                if (enemy->timeOfLastHit > 0 && getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
                {
                    enemy->sprite->visibility = true;
                }

                if (enemy->rect.y > 250 || enemy->rect.y < -100 || enemy->rect.x > 440 || enemy->rect.x < -128)
                {
                    bouncer_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->sprite, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updateBird(void)
{
    if (bird_active_count > 0)
    {
        for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
        {
            ENY_Actor_t *enemy = birdEnemies[i];
            if (enemy->is_enabled)
            {
                if (enemy->rect.y > 0)
                {
                    if (enemy->rect.x > getPlayerRect().x)
                    {
                        enemy->rect.x -= enemy->velocity.y;
                    }
                    else
                    {
                        enemy->rect.x += enemy->velocity.x;
                    }
                }
                if (checkRectangleCollision(enemy->rect, getShotRect()))
                {
                    ENY_handleHitByShot(enemy);
                }
                else if (checkRectangleCollision(enemy->rect, getHitboxRect()))
                {
                    runPlayerHit();
                }

                enemy->rect.y += enemy->velocity.y;
                if (enemy->timeOfLastHit > 0 && getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
                {
                    enemy->sprite->visibility = true;
                }

                if (enemy->rect.y > 250)
                {
                    bird_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->sprite, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updateGrabber(void)
{
    if (grabber_active_count > 0)
    {
        for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
        {
            ENY_Actor_t *enemy = grabberEnemies[i];
            if (enemy->is_enabled)
            {
                enemy->timeAlive++;

                if (enemy->timeAlive % GRABBER_ROCKET_SPAWN_DELAY == 0)
                {
                    ENY_spawnRocketBullet(enemy->rect.x, enemy->rect.y + 5, 3);
                }

                if (checkRectangleCollision(enemy->rect, getShotRect()))
                {
                    ENY_handleHitByShot(enemy);
                }
                else if (checkRectangleCollision(enemy->rect, getHitboxRect()))
                {
                    runPlayerHit();
                }

                enemy->rect.y += enemy->velocity.y;
                enemy->rect.x += enemy->velocity.x;

                if (enemy->rect.y > 250 || enemy->rect.x > 440 || enemy->rect.x < -128)
                {
                    grabber_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->sprite, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updateAstroid(void)
{
    if (astroid_active_count > 0)
    {
        for (u8 i = 0; i < ASTROID_POOL_SIZE; i++)
        {
            ENY_Actor_t *enemy = astroidEnemies[i];
            if (enemy->is_enabled)
            {
                enemy->rect.y += enemy->velocity.y;

                if (checkRectangleCollision(enemy->rect, getShotRect()))
                {
                    ENY_handleHitByShot(enemy);
                }
                else if (checkRectangleCollision(enemy->rect, getHitboxRect()))
                {
                    runPlayerHit();
                }

                enemy->rect.y += enemy->velocity.y;
                enemy->rect.x += enemy->velocity.x;

                if (enemy->rect.y > 250)
                {
                    astroid_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->sprite, astroidEnemies[i]->rect.x, astroidEnemies[i]->rect.y);
        };
    }
}

static void updateCircleBullets(void)
{
    if (circle_bullet_active_count > 0)
    {
        for (u8 i = 0; i < CIRCLE_BULLETS_POOL_SIZE; i++)
        {
            Actor_t *bullet = circleBullets[i];
            if (bullet->is_enabled)
            {
                if (checkRectangleCollision(bullet->rect, getHitboxRect()))
                {
                    runPlayerHit();
                }

                bullet->rect.y += bullet->velocity.y;
                bullet->rect.x += bullet->velocity.x;

                if (bullet->rect.y < -100 || bullet->rect.y > 250 || bullet->rect.x > 400 || bullet->rect.x < -80)
                {
                    circle_bullet_active_count--;
                    ENY_reset_bullet(bullet);
                }
            }
            SPR_setPosition(bullet->sprite, bullet->rect.x, bullet->rect.y);
        }
    }
}

static void updateRocketBullets(void)
{
    if (rocket_bullet_active_count > 0)
    {
        for (u8 i = 0; i < ROCKET_BULLET_POOL_SIZE; i++)
        {
            Actor_t *bullet = rocketBullets[i];
            if (bullet->is_enabled)
            {
                if (checkRectangleCollision(bullet->rect, getHitboxRect()))
                {
                    runPlayerHit();
                }

                bullet->rect.y += bullet->velocity.y;

                if (bullet->rect.y < -100 || bullet->rect.y > 250)
                {
                    rocket_bullet_active_count--;
                    ENY_reset_bullet(bullet);
                }
            }
            SPR_setPosition(bullet->sprite, bullet->rect.x, bullet->rect.y);
        }
    }
}

void update(void)
{
    updateBird();
    updateGrabber();
    updateAstroid();
    updateCircleBullets();
    updateRocketBullets();
    updateBouncer();
    updateFloater();
    updatePopcorn();
}

void ENY_init(void)
{
    for (u8 i = 0; i < BOUNCER_POOL_COUNT; i++)
    {
        bouncerEnemies[i] = createBouncer();
    }

    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        birdEnemies[i] = createBird();
    }

    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        grabberEnemies[i] = createGrabber();
    }

    for (u8 i = 0; i < ASTROID_POOL_SIZE; i++)
    {
        astroidEnemies[i] = createAstroid();
    }

    for (u8 i = 0; i < CIRCLE_BULLETS_POOL_SIZE; i++)
    {
        circleBullets[i] = CreateCircleBullet();
    }

    for (u8 i = 0; i < ROCKET_BULLET_POOL_SIZE; i++)
    {
        rocketBullets[i] = CreateRocketBullet();
    }

    for (u8 i = 0; i < FLOATER_POOL_COUNT; i++)
    {
        floaterEnemies[i] = createFloater();
    }

    for (u8 i = 0; i < POPCORN_POOL_COUNT; i++)
    {
        popcornEnemies[i] = createPopcorn();
    }

    addTickFunc(update, TRUE);
}

void ENY_destruct(void)
{
    // Destory all enemies here

    for (u8 i = 0; i < BOUNCER_POOL_COUNT; i++)
    {
        ENY_destroyEnemy(bouncerEnemies[i]);
    }

    for (u8 i = 0; i < BIRD_POOL_COUNT; i++)
    {
        ENY_destroyEnemy(birdEnemies[i]);
    }
    for (u8 i = 0; i < GRABBER_POOL_SIZE; i++)
    {
        ENY_destroyEnemy(grabberEnemies[i]);
    }

    for (u8 i = 0; i < ASTROID_POOL_SIZE; i++)
    {
        ENY_destroyEnemy(astroidEnemies[i]);
    }

    for (u8 i = 0; i < FLOATER_POOL_COUNT; i++)
    {
        ENY_destroyEnemy(floaterEnemies[i]);
    }

    for (u8 i = 0; i < POPCORN_POOL_COUNT; i++)
    {
        ENY_destroyEnemy(popcornEnemies[i]);
    }

    for (u8 i = 0; i < CIRCLE_BULLETS_POOL_SIZE; i++)
    {
        ENY_destroyBullet(circleBullets[i]);
    }

    for (u8 i = 0; i < ROCKET_BULLET_POOL_SIZE; i++)
    {
        ENY_destroyBullet(rocketBullets[i]);
    }
}
