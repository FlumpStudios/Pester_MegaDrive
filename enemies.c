#include "enemies.h"
#include "game_script.h"
#include "audio.h"

// defines
#define MAX_POOL_SIZE 15
#define ENEMY_HIT_FLASH_TIME 3

#define GRABBER_ROCKET_SPAWN_DELAY 50
#define BOUNCER_BULLET_SPAWN_DELAY 150
#define FLOATER_BULLET_SPAWN_DELAY 75

static u8 floater_pool_count = 0;
static u8 popcorn_pool_count = 0;
static u8 bouncer_pool_count = 0;
static u8 bird_pool_count = 0;
static u8 grabber_pool_count = 0;
static u8 astroid_pool_count = 0;
static u8 circle_bullet_pool_count = 0;
static u8 rocket_bullet_pool_size = 0;

static u8 bird_active_count = 0;
static u8 grabber_active_count = 0;
static u8 astroid_active_count = 0;
static u8 circle_bullet_active_count = 0;
static u8 rocket_bullet_active_count = 0;
static u8 bouncer_active_count = 0;
static u8 floater_active_count = 0;
static u8 popcorn_active_count = 0;

static u8 bird_current_pool_index = 0;
static u8 grabber_current_pool_index = 0;
static u8 astroid_current_pool_index = 0;
static u8 circle_bullet_current_pool_index = 0;
static u8 rocket_bullet_current_pool_index = 0;
static u8 bouncer_current_pool_index = 0;
static u8 floater_current_pool_index = 0;
static u8 popcorn_current_pool_index = 0;


// Enemy
static ENY_Actor_t *birdEnemies[MAX_POOL_SIZE];
static ENY_Actor_t *grabberEnemies[MAX_POOL_SIZE];
static ENY_Actor_t *astroidEnemies[MAX_POOL_SIZE];
static ENY_Actor_t *bouncerEnemies[MAX_POOL_SIZE];
static ENY_Actor_t *floaterEnemies[MAX_POOL_SIZE];
static ENY_Actor_t *popcornEnemies[MAX_POOL_SIZE];

// Bosses
static ENY_Actor_t *boss_1;

// Bullets
static Actor_t *circleBullets[MAX_POOL_SIZE];
static Actor_t *rocketBullets[MAX_POOL_SIZE];


static ENY_Actor_t *createBoss1(void)
{
    ENY_Actor_t *result = ENY_new_enemy_actor();

    result->initial_health = 100;
    result->rect.height = 32;
    result->rect.width = 64;
    result->worth = 500;

    result->spriteSlot1 = SPR_addSprite(&boss1Left, result->rect.x, result->rect.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    result->spriteSlot2 = SPR_addSprite(&boss1Left, result->rect.x + 32, result->rect.y, TILE_ATTR(PAL1, 0, FALSE, TRUE));

    ENY_reset(result);
    return result;
}

static ENY_Actor_t *createPopcorn(void)
{
    ENY_Actor_t *result = ENY_new_enemy_actor();

    result->initial_health = 1;
    result->rect.height = 16;
    result->rect.width = 16;
    result->worth = 5;

    result->spriteSlot1 = SPR_addSprite(&popcorn, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    ENY_reset(result);
    return result;
}

static ENY_Actor_t *createFloater(void)
{
    ENY_Actor_t *result = ENY_new_enemy_actor();

    result->initial_health = 2;
    result->rect.height = 16;
    result->rect.width = 32;
    result->worth = 25;

    result->spriteSlot1 = SPR_addSprite(&floater, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    ENY_reset(result);
    return result;
}

static ENY_Actor_t *createBouncer(void)
{
    ENY_Actor_t *result = ENY_new_enemy_actor();

    result->initial_health = 4;
    result->rect.height = 16;
    result->rect.width = 32;
    result->worth = 50;

    result->spriteSlot1 = SPR_addSprite(&bouncer, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    ENY_reset(result);
    return result;
}

static ENY_Actor_t *createBird(void)
{
    ENY_Actor_t *result = ENY_new_enemy_actor();

    result->initial_health = 3;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 10;

    result->spriteSlot1 = SPR_addSprite(&bird, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    ENY_reset(result);
    return result;
}

static ENY_Actor_t *createGrabber(void)
{
    ENY_Actor_t *result = ENY_new_enemy_actor();

    result->initial_health = 1;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 5;

    result->spriteSlot1 = SPR_addSprite(&grabber, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    ENY_reset(result);
    return result;
}

static ENY_Actor_t *createAstroid(void)
{
    ENY_Actor_t *result = ENY_new_enemy_actor();

    result->initial_health = 1;
    result->rect.height = 32;
    result->rect.width = 32;
    result->worth = 5;

    result->spriteSlot1 = SPR_addSprite(&astroid, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    ENY_reset(result);

    return result;
}

static Actor_t *CreateCircleBullet(void)
{
    Actor_t *result = CMN_new_actor();

    result->rect.height = 16;
    result->rect.width = 16;

    result->spriteSlot1 = SPR_addSprite(&enemyBullet, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    ENY_reset_bullet(result);
    return result;
}

static Actor_t *CreateRocketBullet(void)
{
    Actor_t *result = CMN_new_actor();

    result->rect.height = 16;
    result->rect.width = 8;

    result->spriteSlot1 = SPR_addSprite(&enemyRocket, result->rect.x, result->rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    ENY_reset_bullet(result);
    return result;
}

void ENY_spawnRocketBullet(s16 x, s16 y, s16 ySpeed)
{
    ENY_runBulletSpawnSetup(rocketBullets[rocket_bullet_current_pool_index], x, y, 0, ySpeed);
    rocket_bullet_active_count++;
    rocket_bullet_current_pool_index++;
    if (rocket_bullet_current_pool_index >= rocket_bullet_pool_size)
    {
        rocket_bullet_current_pool_index = 0;
    }
}

void ENY_spawncircleBullet(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    ENY_runBulletSpawnSetup(circleBullets[circle_bullet_current_pool_index], x, y, xSpeed, ySpeed);
    circle_bullet_active_count++;
    circle_bullet_current_pool_index++;
    if (circle_bullet_current_pool_index >= circle_bullet_pool_count)
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

void ENY_spawn_boss_1()
{
    boss_1 = createBoss1();
    ENY_runSpawnSetup(boss_1, 125, -32, 0, 0);
}

void ENY_spawnBouncer(s16 x, s16 y, s16 xSpeed, s16 ySpeed, u16 lifeTime)
{
    if (bouncer_active_count < bouncer_pool_count)
    {
        bouncer_active_count++;
        ENY_runSpawnSetup(bouncerEnemies[bouncer_current_pool_index], x, y, xSpeed, ySpeed);
        bouncerEnemies[bouncer_current_pool_index]->lifeTime = lifeTime;
        bouncer_current_pool_index++;
    }

    if (bouncer_current_pool_index >= bouncer_pool_count)
    {
        bouncer_current_pool_index = 0;
    }
}

void ENY_spawnPopcorn(s16 x, s16 y, s16 xSpeed, s16 ySpeed, u8 variation)
{
    if (popcorn_active_count < popcorn_pool_count)
    {
        popcorn_active_count++;
        ENY_runSpawnSetup(popcornEnemies[popcorn_current_pool_index], x, y, xSpeed, ySpeed);
        popcornEnemies[popcorn_current_pool_index]->variationId = variation;
        SPR_setAnim(popcornEnemies[popcorn_current_pool_index]->spriteSlot1, variation);

        popcorn_current_pool_index++;
    }

    if (popcorn_current_pool_index >= popcorn_pool_count)
    {
        popcorn_current_pool_index = 0;
    }
}

void ENY_spawnFloater(s16 x, s16 y, s16 ySpeed, u8 variation)
{
    if (floater_active_count < floater_pool_count)
    {
        floater_active_count++;
        ENY_runSpawnSetup(floaterEnemies[floater_current_pool_index], x, y, 0, ySpeed);
        floaterEnemies[floater_current_pool_index]->variationId = variation;

        SPR_setAnim(floaterEnemies[floater_current_pool_index]->spriteSlot1, variation);

        floater_current_pool_index++;
    }

    if (floater_current_pool_index >= floater_pool_count)
    {
        floater_current_pool_index = 0;
    }
}

void ENY_spawnBird(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    if (bird_active_count < bird_pool_count)
    {
        bird_active_count++;
        ENY_runSpawnSetup(birdEnemies[bird_current_pool_index], x, y, xSpeed, ySpeed);
        bird_current_pool_index++;
    }

    if (bird_current_pool_index >= bird_pool_count)
    {
        bird_current_pool_index = 0;
    }
}

void ENY_spawnGrabber(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    if (grabber_active_count < grabber_pool_count)
    {
        grabber_active_count++;
        ENY_runSpawnSetup(grabberEnemies[grabber_current_pool_index], x, y, xSpeed, ySpeed);
        grabber_current_pool_index++;
    }

    if (grabber_current_pool_index >= grabber_pool_count)
    {
        grabber_current_pool_index = 0;
    }
}

void ENY_spawnAstroid(s16 x, s16 y, s16 xSpeed, s16 ySpeed)
{
    if (astroid_active_count < astroid_pool_count)
    {
        astroid_active_count++;
        ENY_runSpawnSetup(astroidEnemies[astroid_current_pool_index], x, y, xSpeed, ySpeed);
        astroid_current_pool_index++;
    }

    if (astroid_current_pool_index >= astroid_pool_count)
    {
        astroid_current_pool_index = 0;
    }
}

void ENY_resetAllBullets(void)
{
    for (u8 i = 0; i < circle_bullet_pool_count; i++)
    {
        ENY_reset_bullet(circleBullets[i]);
    }

    for (u8 i = 0; i < rocket_bullet_pool_size; i++)
    {
        ENY_reset_bullet(rocketBullets[i]);
    }
}

void ENY_resetAllEnemies(void)
{
    for (u8 i = 0; i < bird_pool_count; i++)
    {
        ENY_reset(birdEnemies[i]);
        SPR_setPosition(birdEnemies[i]->spriteSlot1, birdEnemies[i]->rect.x, birdEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < grabber_pool_count; i++)
    {
        ENY_reset(grabberEnemies[i]);
        SPR_setPosition(grabberEnemies[i]->spriteSlot1, grabberEnemies[i]->rect.x, grabberEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < astroid_pool_count; i++)
    {
        ENY_reset(astroidEnemies[i]);
        SPR_setPosition(astroidEnemies[i]->spriteSlot1, astroidEnemies[i]->rect.x, astroidEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < bouncer_pool_count; i++)
    {
        ENY_reset(bouncerEnemies[i]);
        SPR_setPosition(bouncerEnemies[i]->spriteSlot1, bouncerEnemies[i]->rect.x, bouncerEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < floater_pool_count; i++)
    {
        ENY_reset(floaterEnemies[i]);
        SPR_setPosition(floaterEnemies[i]->spriteSlot1, floaterEnemies[i]->rect.x, floaterEnemies[i]->rect.y);
    }

    for (u8 i = 0; i < popcorn_pool_count; i++)
    {
        ENY_reset(popcornEnemies[i]);
        SPR_setPosition(popcornEnemies[i]->spriteSlot1, popcornEnemies[i]->rect.x, popcornEnemies[i]->rect.y);
    }

    SPR_setPosition(boss_1->spriteSlot1, boss_1->rect.x, boss_1->rect.y);
    SPR_setPosition(boss_1->spriteSlot2, boss_1->rect.x + 32, boss_1->rect.y);

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

static void updateBoss1(void)
{
    if (boss_1 != NULL)
    {
        ENY_Actor_t *enemy = boss_1;

        enemy->timeAlive++;

        // intro
        if (enemy->timeAlive % 2 == 0)
        {
            if (enemy->timeAlive < 200)
            {
                enemy->rect.y += 1;
            }
            else if (enemy->timeAlive > 200)
            {
                static bool bounced = false;
                static bool bouncedTop = false;
                enemy->velocity.x = 1;
                enemy->velocity.y = 1;

                if (enemy->current_health > 100)
                {
                    enemy->velocity.x = 1;
                    if (enemy->timeAlive % 100 == 0)
                    {
                        ENY_spawncircleBullets_squarepattern(enemy->rect.x + 24, enemy->rect.y + 16);
                    }
                }
                if (enemy->current_health > 75)
                {
                    enemy->velocity.x = 2;
                    if (enemy->timeAlive % 100 == 0 && enemy->timeAlive % 200 != 0)
                    {
                        ENY_spawncircleBullets_squarepattern(enemy->rect.x + 24, enemy->rect.y + 16);
                    }
                    if (enemy->timeAlive % 200 == 0)
                    {
                        ENY_spawncircleBullets_forkedpattern(enemy->rect.x + 24, enemy->rect.y + 20);
                    }
                }
                else if (enemy->current_health > 50)
                {
                    if (enemy->timeAlive % 100 == 0)
                    {
                        ENY_spawncircleBullets_tripplepattern(enemy->rect.x + 24, enemy->rect.y + 20);
                    }
                    enemy->velocity.x = 4;
                }
                else if (enemy->current_health > 25)
                {
                    if (enemy->timeAlive % 20 == 0)
                    {
                        ENY_spawnRocketBullet(enemy->rect.x + 24, enemy->rect.y + 20, 2);
                    }

                    enemy->velocity.x = 6;
                }
                else if (enemy->current_health > 1)
                {
                    if (enemy->timeAlive % 50 == 0)
                    {
                        ENY_spawnRocketBullet(enemy->rect.x + 24, enemy->rect.y + 20, 2);
                    }

                    if (enemy->timeAlive % 110 == 0)
                    {
                        ENY_spawncircleBullets_forkedpattern(enemy->rect.x + 24, enemy->rect.y + 20);
                    }

                    enemy->velocity.x = 8;
                }
                else
                {
                    enemy->velocity.x = 0;
                    enemy->velocity.y = 0;
                }

                Edges_t screenLimits = CMN_get_screen_limits();

                if (enemy->timeAlive % 40 == 0)
                {
                    bouncedTop = !bouncedTop;
                }
                if (bounced)
                {
                    enemy->rect.x += enemy->velocity.x;
                }
                else
                {
                    enemy->rect.x -= enemy->velocity.x;
                }

                if (bouncedTop)
                {
                    enemy->rect.y += enemy->velocity.y;
                }
                else
                {
                    enemy->rect.y -= enemy->velocity.y;
                }

                if (enemy->rect.x < screenLimits.left)
                {
                    bounced = true;
                }
                else if (enemy->rect.x + 64 > screenLimits.right)
                {
                    bounced = false;
                }
            }
        }

        static u8 deathTime = 0;
        // Catch before no health so we can handle the death anim here...and don't allow collision when entering
        if (enemy->current_health > 1)
        {
            if (enemy->timeAlive > 200)
            {
                if (ENY_checkShotCollision(enemy))
                {
                    AUD_play_hit();
                }
                
                if (CLS_checkRectangleCollision(enemy->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }
            }
        }
        else
        {
            deathTime++;

            if (deathTime < 26)
            {
                VX_spawnLargeExplosion(enemy->rect.x, enemy->rect.y, deathTime, 5);
            }

            if (deathTime == 25)
            {
                ENY_kill(enemy);
            }

            // Little buffer before starting next level
            if (deathTime == 100)
            {
                SCR_end_current_level();
            }
        }

        if (enemy != NULL)
        {
            if (enemy->timeOfLastHit > 0 && GST_getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
            {
                enemy->spriteSlot1->visibility = true;
                enemy->spriteSlot2->visibility = true;
            }
            SPR_setPosition(enemy->spriteSlot1, enemy->rect.x, enemy->rect.y);
            SPR_setPosition(enemy->spriteSlot2, enemy->rect.x + 32, enemy->rect.y);
        }
    }
}

static void updateFloater(void)
{
    if (floater_active_count > 0)
    {
        for (u8 i = 0; i < floater_pool_count; i++)
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
                        if (enemy->variationId == VARIATION_FLOATER_WITH_FORKED_BULLETS && enemy->rect.y < 100)
                        {
                            ENY_spawncircleBullets_forkedpattern(enemy->rect.x + 8, enemy->rect.y + 3);
                        }
                        else if (enemy->variationId == VARIATION_FLOATER_WITH_SIDE_BULLETS)
                        {
                            ENY_spawncircleBullets_sidepattern(enemy->rect.x + 8, enemy->rect.y + 3, 2);
                        }
                    }
                }
                
                ENY_checkShotCollision(enemy);

                if (CLS_checkRectangleCollision(enemy->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }

                if (enemy->timeOfLastHit > 0 && GST_getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
                {
                    enemy->spriteSlot1->visibility = true;
                }

                if (enemy->rect.y > 250 || enemy->rect.x > 440 || enemy->rect.x < -128)
                {
                    floater_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->spriteSlot1, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updatePopcorn(void)
{
    if (popcorn_active_count > 0)
    {
        for (u8 i = 0; i < popcorn_pool_count; i++)
        {
            ENY_Actor_t *enemy = popcornEnemies[i];
            if (enemy->is_enabled)
            {
                enemy->timeAlive++;

                // Custom behavior for variations 2+
                if (enemy->variationId >= 2)
                {
                    if (enemy->timeAlive % VARIATION_POPCORN_CENTRE /*2*/ == 0)
                    {
                        if (enemy->variationId == VARIATION_POPCORN_CENTRE)
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

                        if (enemy->variationId == VARIATION_SNAKE_CENTRE)
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

                        if (enemy->variationId == VARIATION_SNAKE_LEFT || enemy->variationId == VARIATION_SNAKE_RIGHT)
                        {
                            if (enemy->timeAlive % 100 == 0)
                            {
                                ENY_spawnRocketBullet(enemy->rect.x, enemy->rect.y + 5, 3);
                            }

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
                ENY_checkShotCollision(enemy);
                if (CLS_checkRectangleCollision(enemy->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }

                if (enemy->timeOfLastHit > 0 && GST_getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
                {
                    enemy->spriteSlot1->visibility = true;
                }

                if (enemy->rect.y > 250 || enemy->rect.y < -100 || enemy->rect.x > 440 || enemy->rect.x < -128)
                {
                    popcorn_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->spriteSlot1, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updateBouncer(void)
{
    if (bouncer_active_count > 0)
    {
        for (u8 i = 0; i < bouncer_pool_count; i++)
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
                ENY_checkShotCollision(enemy);
                if (CLS_checkRectangleCollision(enemy->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }

                if (enemy->timeOfLastHit > 0 && GST_getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
                {
                    enemy->spriteSlot1->visibility = true;
                }

                if (enemy->rect.y > 250 || enemy->rect.y < -100 || enemy->rect.x > 440 || enemy->rect.x < -128)
                {
                    bouncer_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->spriteSlot1, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updateBird(void)
{
    if (bird_active_count > 0)
    {
        for (u8 i = 0; i < bird_pool_count; i++)
        {
            ENY_Actor_t *enemy = birdEnemies[i];
            if (enemy->is_enabled)
            {
                if (enemy->rect.y > 0)
                {
                    if (enemy->rect.x > PLY_getPlayerRect().x)
                    {
                        enemy->rect.x -= enemy->velocity.y;
                    }
                    else
                    {
                        enemy->rect.x += enemy->velocity.x;
                    }
                }
                ENY_checkShotCollision(enemy);
                if (CLS_checkRectangleCollision(enemy->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }

                enemy->rect.y += enemy->velocity.y;
                if (enemy->timeOfLastHit > 0 && GST_getLevelTime() > (enemy->timeOfLastHit + ENEMY_HIT_FLASH_TIME))
                {
                    enemy->spriteSlot1->visibility = true;
                }

                if (enemy->rect.y > 250)
                {
                    bird_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->spriteSlot1, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updateGrabber(void)
{
    if (grabber_active_count > 0)
    {
        for (u8 i = 0; i < grabber_pool_count; i++)
        {
            ENY_Actor_t *enemy = grabberEnemies[i];
            if (enemy->is_enabled)
            {
                enemy->timeAlive++;

                if (enemy->timeAlive % GRABBER_ROCKET_SPAWN_DELAY == 0)
                {
                    ENY_spawnRocketBullet(enemy->rect.x, enemy->rect.y + 5, 3);
                }

                
                ENY_checkShotCollision(enemy);
                if (CLS_checkRectangleCollision(enemy->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }

                enemy->rect.y += enemy->velocity.y;
                enemy->rect.x += enemy->velocity.x;

                if (enemy->rect.y > 250 || enemy->rect.x > 440 || enemy->rect.x < -128)
                {
                    grabber_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->spriteSlot1, enemy->rect.x, enemy->rect.y);
        };
    }
}

static void updateAstroid(void)
{
    if (astroid_active_count > 0)
    {
        for (u8 i = 0; i < astroid_pool_count; i++)
        {
            ENY_Actor_t *enemy = astroidEnemies[i];
            if (enemy->is_enabled)
            {
                enemy->rect.y += enemy->velocity.y;

                ENY_checkShotCollision(enemy);
                if (CLS_checkRectangleCollision(enemy->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }

                enemy->rect.y += enemy->velocity.y;
                enemy->rect.x += enemy->velocity.x;

                if (enemy->rect.y > 250)
                {
                    astroid_active_count--;
                    ENY_reset(enemy);
                }
            }
            SPR_setPosition(enemy->spriteSlot1, astroidEnemies[i]->rect.x, astroidEnemies[i]->rect.y);
        };
    }
}

static void updateCircleBullets(void)
{
    if (circle_bullet_active_count > 0)
    {
        for (u8 i = 0; i < circle_bullet_pool_count; i++)
        {
            Actor_t *bullet = circleBullets[i];
            if (bullet->is_enabled)
            {
                if (CLS_checkRectangleCollision(bullet->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }

                bullet->rect.y += bullet->velocity.y;
                bullet->rect.x += bullet->velocity.x;

                if (bullet->rect.y < -100 || bullet->rect.y > 250 || bullet->rect.x > 400 || bullet->rect.x < -80)
                {
                    circle_bullet_active_count--;
                    ENY_reset_bullet(bullet);
                }
            }
            SPR_setPosition(bullet->spriteSlot1, bullet->rect.x, bullet->rect.y);
        }
    }
}

static void updateRocketBullets(void)
{
    if (rocket_bullet_active_count > 0)
    {
        for (u8 i = 0; i < rocket_bullet_pool_size; i++)
        {
            Actor_t *bullet = rocketBullets[i];
            if (bullet->is_enabled)
            {
                if (CLS_checkRectangleCollision(bullet->rect, PLY_getHitboxRect()))
                {
                    PLY_runPlayerHit();
                }

                bullet->rect.y += bullet->velocity.y;

                if (bullet->rect.y < -100 || bullet->rect.y > 250)
                {
                    rocket_bullet_active_count--;
                    ENY_reset_bullet(bullet);
                }
            }
            SPR_setPosition(bullet->spriteSlot1, bullet->rect.x, bullet->rect.y);
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
    updateBoss1();
}

void ENY_createBouncerPool(u8 count)
{
    if (count > MAX_POOL_SIZE)
    {
        count = MAX_POOL_SIZE;
    }

    bouncer_pool_count = count;
    for (u8 i = 0; i < bouncer_pool_count; i++)
    {
        bouncerEnemies[i] = createBouncer();
    }
}

void ENY_createBirdPool(u8 count)
{
    if (count > MAX_POOL_SIZE)
    {
        count = MAX_POOL_SIZE;
    }

    bird_pool_count = count;
    for (u8 i = 0; i < bird_pool_count; i++)
    {
        birdEnemies[i] = createBird();
    }
}

void ENY_createGrabberPool(u8 count)
{
    if (count > MAX_POOL_SIZE)
    {
        count = MAX_POOL_SIZE;
    }

    grabber_pool_count = count;
    for (u8 i = 0; i < grabber_pool_count; i++)
    {
        grabberEnemies[i] = createGrabber();
    }
}

void ENY_createAstroidPool(u8 count)
{
    if (count > MAX_POOL_SIZE)
    {
        count = MAX_POOL_SIZE;
    }

    astroid_pool_count = count;
    for (u8 i = 0; i < astroid_pool_count; i++)
    {
        astroidEnemies[i] = createAstroid();
    }
}

void ENY_createCircleBulletPool(u8 count)
{
    if (count > MAX_POOL_SIZE)
    {
        count = MAX_POOL_SIZE;
    }

    circle_bullet_pool_count = count;

    for (u8 i = 0; i < circle_bullet_pool_count; i++)
    {
        circleBullets[i] = CreateCircleBullet();
    }
}

void ENY_createRocketBulletPool(u8 count)
{
    if (count > MAX_POOL_SIZE)
    {
        count = MAX_POOL_SIZE;
    }

    rocket_bullet_pool_size = count;

    for (u8 i = 0; i < rocket_bullet_pool_size; i++)
    {
        rocketBullets[i] = CreateRocketBullet();
    }
}

void ENY_createFloaterPool(u8 count)
{
    if (count > MAX_POOL_SIZE)
    {
        count = MAX_POOL_SIZE;
    }

    floater_pool_count = count;

    for (u8 i = 0; i < floater_pool_count; i++)
    {
        floaterEnemies[i] = createFloater();
    }
}

void ENY_createPopcornPool(u8 count)
{
    if (count > MAX_POOL_SIZE)
    {
        count = MAX_POOL_SIZE;
    }

    popcorn_pool_count = count;

    for (u8 i = 0; i < popcorn_pool_count; i++)
    {
        popcornEnemies[i] = createPopcorn();
    }
}

void reset_statics(void)
{
    floater_pool_count = 0;
    popcorn_pool_count = 0;
    bouncer_pool_count = 0;
    bird_pool_count = 0;
    grabber_pool_count = 0;
    astroid_pool_count = 0;
    circle_bullet_pool_count = 0;
    rocket_bullet_pool_size = 0;

    bird_active_count = 0;
    grabber_active_count = 0;
    astroid_active_count = 0;
    circle_bullet_active_count = 0;
    rocket_bullet_active_count = 0;
    bouncer_active_count = 0;
    floater_active_count = 0;
    popcorn_active_count = 0;

    bird_current_pool_index = 0;
    grabber_current_pool_index = 0;
    astroid_current_pool_index = 0;
    circle_bullet_current_pool_index = 0;
    rocket_bullet_current_pool_index = 0;
    bouncer_current_pool_index = 0;
    floater_current_pool_index = 0;
    popcorn_current_pool_index = 0;
}

void ENY_init(void)
{
    for (u8 i = 0; i < MAX_POOL_SIZE; i++)
    {
        bouncerEnemies[i] = NULL;
        birdEnemies[i] = NULL;
        grabberEnemies[i] = NULL;
        astroidEnemies[i] = NULL;
        floaterEnemies[i] = NULL;
        popcornEnemies[i] = NULL;
        circleBullets[i] = NULL;
        rocketBullets[i] = NULL;
    }

    boss_1 = NULL;

    static bool tickFunctionAdded = false;
    if (!tickFunctionAdded)
    {
        addTickFunc(update, TRUE);
        tickFunctionAdded = true;
    }
}

void ENY_destruct_enemies(void)
{
    ENY_destroyEnemy(boss_1);
    boss_1 = NULL;

    for (u8 i = 0; i < MAX_POOL_SIZE; i++)
    {
        ENY_destroyEnemy(bouncerEnemies[i]);
        bouncerEnemies[i] = NULL;

        ENY_destroyEnemy(birdEnemies[i]);
        birdEnemies[i] = NULL;

        ENY_destroyEnemy(grabberEnemies[i]);
        grabberEnemies[i] = NULL;

        ENY_destroyEnemy(astroidEnemies[i]);
        astroidEnemies[i] = NULL;

        ENY_destroyEnemy(floaterEnemies[i]);
        floaterEnemies[i] = NULL;

        ENY_destroyEnemy(popcornEnemies[i]);
        popcornEnemies[i] = NULL;
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

void ENY_destruct_bullets(void)
{
    circle_bullet_active_count = 0;
    rocket_bullet_active_count = 0;
    circle_bullet_current_pool_index = 0;
    rocket_bullet_current_pool_index = 0;

    for (u8 i = 0; i < MAX_POOL_SIZE; i++)
    {
        ENY_destroyBullet(circleBullets[i]);
        ENY_destroyBullet(rocketBullets[i]);
    }
}
