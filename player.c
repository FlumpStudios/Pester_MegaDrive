#include "player.h"
#include "game_update_observable.h"
#include "gamestate.h"
#include "common.h"
#include "audio.h"
// player struct should have been an actor type, but I ballsed it up :(

#define DEATH_TIMEOUT 100

typedef struct ply
{
    Actor_t ship;
    Actor_t shot;
    Actor_t satellite_shot1;
    Actor_t satellite_shot2;
    Rectangle_t hitbox_rect;
    Sprite *hit_box_spr;
    Sprite *left_satellite;
    Sprite *right_satellite;
} Player_t;

static Player_t *player = NULL;
static bool are_bondary_checks_enabled;
static bool is_player_in_death_state;
static u16 death_ticker;
static bool hasPlayedGameOverSound;
static bool satellites_enabled;

void PLY_set_boundary_checks_enabled(bool enabled)
{
    are_bondary_checks_enabled = enabled;
}

static void reset_after_death(void)
{
    death_ticker = 0;

    if (GST_getLivesCount() > 0)
    {
        is_player_in_death_state = false;
        GST_resetChain();
        GST_removeLife();
        ENY_resetAllEnemies();
        ENY_resetAllBullets();
        CTR_set_locked_controls(false);
        player->ship.spriteSlot1->visibility = true;
        player->hit_box_spr->visibility = true;

        if (satellites_enabled)
        {
            player->left_satellite->visibility = true;
            player->right_satellite->visibility = true;
        }
        else
        {
            player->left_satellite->visibility = false;
            player->right_satellite->visibility = false;
        }

        player->ship.rect.height = 32;
        player->ship.rect.width = 32;
        player->ship.rect.x = 144;
        player->ship.rect.y = 150;
    }
    else
    {
        if (!hasPlayedGameOverSound)
        {
            AUD_stop_music();
            AUD_play_game_over();
            hasPlayedGameOverSound = true;
        }
        GST_endGame();
    }
}

void PLY_resetPlayer()
{
    death_ticker = 0;
    is_player_in_death_state = false;

    player->ship.speed = 3;
    player->shot.speed = 6;

    player->satellite_shot1.speed = 6;
    player->satellite_shot2.speed = 6;

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

    player->satellite_shot1.rect.x = -DEACTIVATED_POSITION;
    player->satellite_shot1.rect.y = -DEACTIVATED_POSITION;
    player->satellite_shot1.rect.width = 8;
    player->satellite_shot1.rect.height = 8;
    player->satellite_shot1.velocity.x = 0;
    player->satellite_shot1.velocity.y = 0;

    player->satellite_shot2.rect.x = -DEACTIVATED_POSITION;
    player->satellite_shot2.rect.y = -DEACTIVATED_POSITION;
    player->satellite_shot2.rect.width = 8;
    player->satellite_shot2.rect.height = 8;
    player->satellite_shot2.velocity.x = 0;
    player->satellite_shot2.velocity.y = 0;

    player->ship.velocity.x = 0;
    player->ship.velocity.y = 0;
    player->ship.spriteSlot1->visibility = true;
    are_bondary_checks_enabled = false;

    if(satellites_enabled)
    {
        player->left_satellite->visibility = true;
        player->right_satellite->visibility = true;
    }
    else
    {
        player->left_satellite->visibility = false;
        player->right_satellite->visibility = false;
    }
}

void PLY_runPlayerHit()
{
    if (!is_player_in_death_state)
    {
        player->left_satellite->visibility = false;
        player->right_satellite->visibility = false;
        player->ship.spriteSlot1->visibility = false;
        player->hit_box_spr->visibility = false;

        AUD_play_player_death();
        CTR_set_locked_controls(true);
        VX_spawnExposion(player->ship.rect);
        is_player_in_death_state = true;
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
    player->hitbox_rect.y = player->ship.rect.y + 12;

    // Set spriteSlot1 position in SGDK
    SPR_setPosition(player->ship.spriteSlot1, player->ship.rect.x, player->ship.rect.y);
    SPR_setPosition(player->hit_box_spr, player->hitbox_rect.x, player->hitbox_rect.y);
    SPR_setPosition(player->left_satellite, player->ship.rect.x - 25, player->ship.rect.y + 12);
    SPR_setPosition(player->right_satellite, player->ship.rect.x + 41, player->ship.rect.y + 12);
}

void PLY_disableShot(void)
{
    player->shot.is_enabled = false;
    player->shot.spriteSlot1->visibility = false;
}

void PLY_resetShot(void)
{
    UI_updateChainDisplay();
    player->shot.rect.y = -30;
    PLY_disableShot();
}

void moveShot(void)
{
    player->shot.rect.x += player->shot.velocity.x;
    player->shot.rect.y += player->shot.velocity.y;
    SPR_setPosition(player->shot.spriteSlot1, player->shot.rect.x, player->shot.rect.y);
}

void moveSataliteShots(void)
{
    player->satellite_shot1.rect.x += player->satellite_shot1.velocity.x;
    player->satellite_shot1.rect.y += player->satellite_shot1.velocity.y;

    player->satellite_shot2.rect.x += player->satellite_shot2.velocity.x;
    player->satellite_shot2.rect.y += player->satellite_shot2.velocity.y;

    SPR_setPosition(player->satellite_shot1.spriteSlot1, player->satellite_shot1.rect.x, player->satellite_shot1.rect.y);
    SPR_setPosition(player->satellite_shot2.spriteSlot1, player->satellite_shot2.rect.x, player->satellite_shot2.rect.y);
}

void PLY_fireShot(void)
{
    player->shot.spriteSlot1->visibility = true;
    player->shot.is_enabled = true;
    player->shot.rect.x = player->ship.rect.x + 12;
    player->shot.rect.y = player->ship.rect.y - 12;

    if (satellites_enabled && PLY_areSataliteShotsOutOfBounds())
    {

        player->satellite_shot1.spriteSlot1->visibility = true;
        player->satellite_shot1.is_enabled = true;
        player->satellite_shot1.rect.x = player->ship.rect.x - 20;
        player->satellite_shot1.rect.y = player->ship.rect.y + 9;
        player->satellite_shot2.spriteSlot1->visibility = true;
        player->satellite_shot2.is_enabled = true;
        player->satellite_shot2.rect.x = player->ship.rect.x + 45;
        player->satellite_shot2.rect.y = player->ship.rect.y + 9;
    }

    AUD_play_player_shot();
}

bool PLY_areSataliteShotsOutOfBounds(void)
{
    return player->satellite_shot1.rect.y <= 4;
}

bool PLY_isShotOutOfBounds(void)
{
    return player->shot.rect.y <= 0;
}

void PLY_destructPlayer(void)
{
    if (player != NULL)
    {
        SPR_releaseSprite(player->ship.spriteSlot1);
        SPR_releaseSprite(player->shot.spriteSlot1);
        SPR_releaseSprite(player->hit_box_spr);
        SPR_releaseSprite(player->left_satellite);
        SPR_releaseSprite(player->right_satellite);
        SPR_releaseSprite(player->satellite_shot1.spriteSlot1);
        SPR_releaseSprite(player->satellite_shot2.spriteSlot1);
        MEM_free(player);
        player = NULL;
    }
}

void PLY_moveLeft(bool seconAxisActive)
{
    if (seconAxisActive)
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

void PLY_enableSatelliteShot(void)
{
    player->satellite_shot1.velocity.y = -player->satellite_shot1.speed;
    player->satellite_shot2.velocity.y = -player->satellite_shot2.speed;

    player->satellite_shot1.velocity.x = -1;
    player->satellite_shot2.velocity.x = 1;

    player->satellite_shot1.is_enabled = true;
    player->satellite_shot2.is_enabled = true;

    player->satellite_shot1.spriteSlot1->visibility = true;
    player->satellite_shot2.spriteSlot1->visibility = true;
}

void PLY_disableShotMovement(void)
{
    player->shot.velocity.y = 0;
}

void PLY_disableSatelliteShot(u8 number)
{
    if (number == 1 || number == 0)
    {
        player->satellite_shot1.is_enabled = false;
        player->satellite_shot1.spriteSlot1->visibility = false;
        player->satellite_shot1.rect.x = -150;
    }

    if (number == 2 || number == 0)
    {
        player->satellite_shot2.is_enabled = false;
        player->satellite_shot2.spriteSlot1->visibility = false;
        player->satellite_shot2.rect.x = -150;
    }
}

Rectangle_t PLY_getPlayerRect(void)
{
    return player->ship.rect;
}

Rectangle_t PLY_getShotRect(void)
{
    return player->shot.rect;
}

Rectangle_t PLY_getSatelliteShot(u8 number)
{
    if (number == 1)
    {
        return player->satellite_shot1.rect;
    }
    else
    {
        return player->satellite_shot2.rect;
    }
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
    if (satellites_enabled)
    {
        moveSataliteShots();
    }

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

    if (PLY_areSataliteShotsOutOfBounds())
    {
        PLY_disableSatelliteShot(0);
    }
    else
    {
        PLY_enableSatelliteShot();
    }

    if (PLY_isShotOutOfBounds())
    {
        if (player->shot.rect.y > -8)
        {

            if (GST_getChain() >= 10)
            {
                AUD_play_lost_chain();
            }

            GST_resetChain();
        }

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
        player->hit_box_spr = SPR_addSprite(&hitBox, player->hitbox_rect.x, player->hitbox_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
        player->ship.spriteSlot1 = SPR_addSprite(&paddle, player->ship.rect.x, player->ship.rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
        player->ship.spriteSlot2 = NULL;
        player->shot.spriteSlot1 = SPR_addSprite(&imgball, player->shot.rect.x, player->shot.rect.y, TILE_ATTR(PAL2, 2, FALSE, FALSE));
        player->shot.spriteSlot2 = NULL;
        player->left_satellite = SPR_addSprite(&satellite, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 2, FALSE, FALSE));
        player->right_satellite = SPR_addSprite(&satellite, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 2, FALSE, FALSE));

        player->satellite_shot1.spriteSlot1 = SPR_addSprite(&satelliteShot, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 2, FALSE, FALSE));
        player->satellite_shot2.spriteSlot1 = SPR_addSprite(&satelliteShot, DEACTIVATED_POSITION, DEACTIVATED_POSITION, TILE_ATTR(PAL2, 2, FALSE, FALSE));
    }

    static bool tickFunctionAdded = false;
    if (!tickFunctionAdded)
    {
        addTickFunc(PLY_update, true);
        tickFunctionAdded = true;
    }

    // init statics
    are_bondary_checks_enabled = false;
    is_player_in_death_state = false;
    death_ticker = 0;
    hasPlayedGameOverSound = false;
    satellites_enabled = false;
}