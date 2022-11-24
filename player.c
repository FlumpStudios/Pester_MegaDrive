#include "player.h"
#include "game_update_observable.h"
#include "gamestate.h"
#include "common.h"
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
static bool are_bondary_checks_enabled = false;
static bool is_player_in_death_state = false;
static u16 death_ticker = 0;

void PLY_set_boundary_checks_enabled(bool enabled)
{
    are_bondary_checks_enabled = enabled;
}

static void reset_after_death(void)
{
    death_ticker = 0;
    is_player_in_death_state = false;

    if (GST_getLivesCount() > 0)
    {
        GST_removeLife();
        ENY_resetAllEnemies();
        ENY_resetAllBullets();
        CTR_set_locked_controls(false);
        player->ship.spriteSlot1->visibility = true;
        player->ship.rect.height = 32;
        player->ship.rect.width = 32;
        player->ship.rect.x = 144;
        player->ship.rect.y = 150;
    }
    else
    {
        GST_endGame();
    }
}

void PLY_resetPlayer()
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
    player->ship.spriteSlot1->visibility = true;
    are_bondary_checks_enabled = false;
}

void PLY_runPlayerHit()
{
    if (!is_player_in_death_state)
    {
        CTR_set_locked_controls(true);
        is_player_in_death_state = true;
        VX_spawnExposion(player->ship.rect);
        player->ship.spriteSlot1->visibility = false;
    }
}

void updatePlayerPosition(void)
{

    Edges_t screenLimits = CMN_get_screen_limits();

    // Set positions
    player->ship.rect.x += player->ship.velocity.x;
    player->ship.rect.y += player->ship.velocity.y;

    // Boundary checks
    if (are_bondary_checks_enabled)
    {
        if (player->ship.rect.x < screenLimits.left)
        {
            player->ship.rect.x = screenLimits.left;
        }

        if (player->ship.rect.x + player->ship.rect.width > screenLimits.right)
        {
            player->ship.rect.x = screenLimits.right - player->ship.rect.width;
        }

        if (player->ship.rect.y < screenLimits.top)
        {
            player->ship.rect.y = screenLimits.top;
        }

        if (player->ship.rect.y + 50 > screenLimits.bottom)
        {
            player->ship.rect.y = screenLimits.bottom - 50;
        }
    }

    // Position the hitbox
    player->hitbox_rect.x = player->ship.rect.x + 12;
    player->hitbox_rect.y = player->ship.rect.y + 16;

    // Set spriteSlot1 position in SGDK
    SPR_setPosition(player->ship.spriteSlot1, player->ship.rect.x, player->ship.rect.y);
}

void PLY_disableShot(void)
{
    player->shot.is_enabled = false;
    player->shot.spriteSlot1->visibility = false;
}

void PLY_resetShot(void)
{
    player->shot.rect.y = -30;
    PLY_disableShot();
}

void moveShot(void)
{
    player->shot.rect.x += player->shot.velocity.x;
    player->shot.rect.y += player->shot.velocity.y;
    SPR_setPosition(player->shot.spriteSlot1, player->shot.rect.x, player->shot.rect.y);
}

void PLY_fireShot(void)
{
    player->shot.spriteSlot1->visibility = true;
    player->shot.is_enabled = true;
    player->shot.rect.x = player->ship.rect.x + 12;
    player->shot.rect.y = player->ship.rect.y - 12;
}

bool PLY_isShotOutOfBounds(void)
{
    return player->shot.rect.y <= 0;
}

void PLY_destructPlayer(void)
{
    SPR_releaseSprite(player->ship.spriteSlot1);
    SPR_releaseSprite(player->shot.spriteSlot1);
    SPR_releaseSprite(player->hit_box_spr);

    MEM_free(player);
}

void PLY_moveLeft(bool seconAxisActive)
{
    if(seconAxisActive)
    {
        player->ship.velocity.x = -player->ship.speed / 1.5;
    }
    else
    {
        player->ship.velocity.x = -player->ship.speed;
    }
    SPR_setAnim(player->ship.spriteSlot1, 1);
}

void PLY_moveRight(bool seconAxisActive)
{
    if (seconAxisActive)
    {
        player->ship.velocity.x = player->ship.speed / 1.5;
    }
    else
    {
        player->ship.velocity.x = player->ship.speed;
    }
    SPR_setAnim(player->ship.spriteSlot1, 2);
}

void PLY_moveUp(bool seconAxisActive)
{
    if (seconAxisActive)
    {
        player->ship.velocity.y = -player->ship.speed / 1.5;
    }
    else
    {
        player->ship.velocity.y = -player->ship.speed;
    }
}

void PLY_moveDown(bool seconAxisActive)
{
    if (seconAxisActive)
    {
        player->ship.velocity.y = player->ship.speed / 1.5;
    }
    else
    {
        player->ship.velocity.y = player->ship.speed;
    }
}

void PLY_haltX(void)
{
    player->ship.velocity.x = 0;
    SPR_setAnim(player->ship.spriteSlot1, 0);
}

void PLY_haltY(void)
{
    player->ship.velocity.y = 0;
}

void PLY_enableShotMovement(void)
{
    player->shot.velocity.y = -player->shot.speed;
}

void PLY_disableShotMovement(void)
{
    player->shot.velocity.y = 0;
}

Rectangle_t PLY_getPlayerRect(void)
{
    return player->ship.rect;
}

Rectangle_t PLY_getShotRect(void)
{
    return player->shot.rect;
}

Rectangle_t PLY_getHitboxRect(void)
{
    return player->hitbox_rect;
}

bool PLY_is_player_shot_enabled(void)
{
    return player->shot.is_enabled;
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
        if (GST_isGamePlaying())
        {
            updatePlayerPosition();
        }
    }

    if (PLY_isShotOutOfBounds())
    {
        PLY_disableShotMovement();
        PLY_resetShot();
    }
    else
    {
        PLY_enableShotMovement();
    }
}

void PLY_init(void)
{
    player = MEM_alloc(sizeof(Player_t));

    if (player != NULL)
    {
        PLY_resetPlayer();
        // player->hit_box_spr = SPR_addSprite(&hitBox, player->hitbox_rect.x, player->hitbox_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
        player->ship.spriteSlot1 = SPR_addSprite(&paddle, player->ship.rect.x, player->ship.rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
        player->ship.spriteSlot2 = NULL;

        player->shot.spriteSlot1 = SPR_addSprite(&imgball, player->shot.rect.x, player->shot.rect.y, TILE_ATTR(PAL2, 2, FALSE, FALSE));
        player->shot.spriteSlot2 = NULL;
    }

    addTickFunc(PLY_update, true);
}