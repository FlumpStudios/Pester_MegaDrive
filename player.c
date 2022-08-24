#include "player.h"

Player_t *player = NULL;

struct Edges screen_limits = {0, 320, 0, 240};

void resetPlayer()
{
    player->speed = 3;
    player->shotSpeed = 6;

    player->player_rect.height = 32;
    player->player_rect.width = 32;
    player->player_rect.x = 144;
    player->player_rect.y = 160;

    player->hitbox_rect.x = 144;
    player->hitbox_rect.y = 160;
    player->hitbox_rect.width = 2;
    player->hitbox_rect.height = 2;

    player->shot_rect.x = 160;
    player->shot_rect.y = -14;
    player->shot_rect.width = 8;
    player->shot_rect.height = 8;

    player->shot_velocity.x = 0;
    player->shot_velocity.y = 0;

    player->velocity.x = 0;
    player->velocity.y = 0;
}

void initialisedPlayer()
{
    if (player == NULL)
    {
        player = (Player_t *)MEM_alloc(sizeof(Player_t));
                
        resetPlayer();

        player->hit_box_spr = SPR_addSprite(&hitBox, player->hitbox_rect.x, player->hitbox_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
        player->player_spr = SPR_addSprite(&paddle, player->player_rect.x, player->player_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
        player->shot_spr = SPR_addSprite(&imgball, player->shot_rect.x, player->shot_rect.y, TILE_ATTR(PAL2, 2, FALSE, FALSE));
    }
}

Player_t *getPlayer()
{
    return player;
}

void updatePlayerPosition()
{
    // Set positions
    player->player_rect.x += player->velocity.x;
    player->player_rect.y += player->velocity.y;

    // Boundary checks
    if (player->player_rect.x < screen_limits.left)
    {
        player->player_rect.x = screen_limits.left;
    }

    if (player->player_rect.x + player->player_rect.width > screen_limits.right)
    {
        player->player_rect.x = screen_limits.right - player->player_rect.width;
    }

    if (player->player_rect.y < screen_limits.top)
    {
        player->player_rect.y = screen_limits.top;
    }

    if (player->player_rect.y + 50 > screen_limits.bottom)
    {
        player->player_rect.y = screen_limits.bottom - 50;
    }

    // Position the hitbox
    player->hitbox_rect.x = player->player_rect.x + 12;
    player->hitbox_rect.y = player->player_rect.y + 16;

    // Set sprite position in SGDK
    SPR_setPosition(player->player_spr, player->player_rect.x, player->player_rect.y);
    SPR_setPosition(player->hit_box_spr, player->hitbox_rect.x, player->hitbox_rect.y);
}

void resetShot()
{
    player->shot_rect.y = DEACTIVATED_POSITION;
}

void moveShot()
{
    player->shot_rect.x += player->shot_velocity.x;
    player->shot_rect.y += player->shot_velocity.y;
    SPR_setPosition(player->shot_spr, player->shot_rect.x, player->shot_rect.y);
}

void fireShot()
{
    player->shot_rect.x = player->player_rect.x + 12;
    player->shot_rect.y = player->player_rect.y;
}

bool isShotOutOfBounds()
{
    return player->shot_rect.y <= -10;
}

void destructPlayer()
{
    MEM_free(player);
}

void moveLeft()
{   
    player->velocity.x = -player->speed;
    SPR_setAnim(player->player_spr, 1);
}

void moveRight()
{   
    player->velocity.x = player->speed;
    SPR_setAnim(player->player_spr, 2);
}

void moveUp()
{   
    player->velocity.y = -player->speed;
}

void moveDown()
{   
    player->velocity.y = player->speed;
}


void haltX()
{
    player->velocity.x = 0;
    SPR_setAnim(player->player_spr, 0);
}

void haltY()
{
    player->velocity.y = 0;
}

void enableShotMovement()
{
    player->shot_velocity.y = -player->shotSpeed;
}

void disableShotMovement()
{
    player->shot_velocity.y = 0;
}

struct Rectangle getPlayerRect()
{
    return player->player_rect;
}

struct Rectangle getShotRect()
{
    return player->shot_rect;
}

struct Rectangle getHitboxRect()
{
    return player->hitbox_rect;
}