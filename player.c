#include "player.h"
#include "game_update_observable.h"
#include "gamestate.h"
// player struct should have been an actor type, but I ballsed it up :(

#define DEATH_TIMEOUT 100

typedef struct ply
{
    Actor_t ship;
    Actor_t shot;
    Rectangle_t hitbox_rect;
    Sprite *hit_box_spr;
} Player_t;

static Player_t *player = NULL;
static Edges_t screen_limits = {0, 320, 0, 240};
static are_bondary_checks_enabled = false;
static is_player_in_death_state = false;
static death_ticker = 0;

void PLY_set_boundary_checks_enabled(bool enabled)
{
    are_bondary_checks_enabled = enabled;
}

static void reset_after_death(void)
{
    death_ticker = 0;
    is_player_in_death_state = false;

    if (getLivesCount() > 0)
    {
        removeLife();
        ENY_resetAllEnemies();
        ENY_resetAllBullets();
        CTR_set_locked_controls(false);
        player->ship.sprite->visibility = true;
        player->ship.rect.height = 32;
        player->ship.rect.width = 32;
        player->ship.rect.x = 144;
        player->ship.rect.y = 150;
    }
    else
    {
        endGame();
    }
}

void resetPlayer()
{
    death_ticker = 0;
    is_player_in_death_state = false;

    player->ship.speed = 3;
    player->shot.speed = 6;

    player->ship.rect.height = 32;
    player->ship.rect.width = 32;
    player->ship.rect.x = 144;
    player->ship.rect.y = -50;

    player->hitbox_rect.x = 144;
    player->hitbox_rect.y = -50;
    player->hitbox_rect.width = 2;
    player->hitbox_rect.height = 2;

    player->shot.rect.x = 160;
    player->shot.rect.y = -14;
    player->shot.rect.width = 8;
    player->shot.rect.height = 16;

    player->shot.velocity.x = 0;
    player->shot.velocity.y = 0;

    player->ship.velocity.x = 0;
    player->ship.velocity.y = 0;
    player->ship.sprite->visibility = true;
    are_bondary_checks_enabled = false;
}

void runPlayerHit()
{
    if (!is_player_in_death_state)
    {
        CTR_set_locked_controls(true);
        is_player_in_death_state = true;
        spawnExposion(player->ship.rect);
        player->ship.sprite->visibility = false;
    }
}

void updatePlayerPosition(void)
{
    // Set positions
    player->ship.rect.x += player->ship.velocity.x;
    player->ship.rect.y += player->ship.velocity.y;

    // Boundary checks
    if (are_bondary_checks_enabled)
    {
        if (player->ship.rect.x < screen_limits.left)
        {
            player->ship.rect.x = screen_limits.left;
        }

        if (player->ship.rect.x + player->ship.rect.width > screen_limits.right)
        {
            player->ship.rect.x = screen_limits.right - player->ship.rect.width;
        }

        if (player->ship.rect.y < screen_limits.top)
        {
            player->ship.rect.y = screen_limits.top;
        }

        if (player->ship.rect.y + 50 > screen_limits.bottom)
        {
            player->ship.rect.y = screen_limits.bottom - 50;
        }
    }

    // Position the hitbox
    player->hitbox_rect.x = player->ship.rect.x + 12;
    player->hitbox_rect.y = player->ship.rect.y + 16;

    // Set sprite position in SGDK
    SPR_setPosition(player->ship.sprite, player->ship.rect.x, player->ship.rect.y);
}

void resetShot(void)
{
    player->shot.rect.y = -27;
}

void moveShot(void)
{
    player->shot.rect.x += player->shot.velocity.x;
    player->shot.rect.y += player->shot.velocity.y;
    SPR_setPosition(player->shot.sprite, player->shot.rect.x, player->shot.rect.y);
}

void fireShot(void)
{
    player->shot.rect.x = player->ship.rect.x + 12;
    player->shot.rect.y = player->ship.rect.y;
}

bool isShotOutOfBounds(void)
{
    return player->shot.rect.y <= 0;
}

void destructPlayer(void)
{
    SPR_releaseSprite(player->ship.sprite);
    SPR_releaseSprite(player->shot.sprite);
    SPR_releaseSprite(player->hit_box_spr);

    MEM_free(player);
}

void moveLeft(void)
{
    player->ship.velocity.x = -player->ship.speed;
    SPR_setAnim(player->ship.sprite, 1);
}

void moveRight(void)
{
    player->ship.velocity.x = player->ship.speed;
    SPR_setAnim(player->ship.sprite, 2);
}

void moveUp(void)
{
    player->ship.velocity.y = -player->ship.speed;
}

void moveDown(void)
{
    player->ship.velocity.y = player->ship.speed;
}

void haltX(void)
{
    player->ship.velocity.x = 0;
    SPR_setAnim(player->ship.sprite, 0);
}

void haltY(void)
{
    player->ship.velocity.y = 0;
}

void enableShotMovement(void)
{
    player->shot.velocity.y = -player->shot.speed;
}

void disableShotMovement(void)
{
    player->shot.velocity.y = 0;
}

Rectangle_t getPlayerRect(void)
{
    return player->ship.rect;
}

Rectangle_t getShotRect(void)
{
    return player->shot.rect;
}

Rectangle_t getHitboxRect(void)
{
    return player->hitbox_rect;
}

void PLY_update(void)
{
    moveShot();

    if (is_player_in_death_state)
    {
        death_ticker++;
        if (death_ticker > DEATH_TIMEOUT)
        {
            reset_after_death();
        }
    }
    else
    {
        if (isGamePlaying())
        {
            updatePlayerPosition();
        }
    }

    if (isShotOutOfBounds())
    {
        disableShotMovement();
        resetShot();
    }
    else
    {
        enableShotMovement();
    }
}

void PLY_init(void)
{
    // Create the player entity
    if (player == NULL)
    {
        player = (Player_t *)MEM_alloc(sizeof(Player_t));

        if (player != NULL)
        {
            resetPlayer();
            // player->hit_box_spr = SPR_addSprite(&hitBox, player->hitbox_rect.x, player->hitbox_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
            player->ship.sprite = SPR_addSprite(&paddle, player->ship.rect.x, player->ship.rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
            player->shot.sprite = SPR_addSprite(&imgball, player->shot.rect.x, player->shot.rect.y, TILE_ATTR(PAL2, 2, FALSE, FALSE));
        }
    }

    // subscribe update funciton to the game ticker
    addTickFunc(PLY_update, true);
}