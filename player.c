#include "player.h"

typedef struct ply
{
    u8 speed;
    u8 shot_speed;
    Vector2_t velocity;
    Vector2_t shot_velocity;
    Rectangle_t player_rect;
    Rectangle_t hitbox_rect;
    Rectangle_t shot_rect;
    Sprite *player_spr;
    Sprite *shot_spr;
    Sprite *hit_box_spr;
} Player_t;

static Player_t *player = NULL;
static Edges_t screen_limits = {0, 320, 0, 240};

void resetPlayer()
{
    player->speed = 3;
    player->shot_speed = 6;

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

void initialisedPlayer(void)
{
    if (player == NULL)
    {
        player = (Player_t *)MEM_alloc(sizeof(Player_t));

        if (player != NULL)
        {
            resetPlayer();
            player->hit_box_spr = SPR_addSprite(&hitBox, player->hitbox_rect.x, player->hitbox_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
            player->player_spr = SPR_addSprite(&paddle, player->player_rect.x, player->player_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
            player->shot_spr = SPR_addSprite(&imgball, player->shot_rect.x, player->shot_rect.y, TILE_ATTR(PAL2, 2, FALSE, FALSE));
        }
    }
}

void updatePlayerPosition(void)
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

void resetShot(void)
{
    player->shot_rect.y = DEACTIVATED_POSITION;
}

void moveShot(void)
{
    player->shot_rect.x += player->shot_velocity.x;
    player->shot_rect.y += player->shot_velocity.y;
    SPR_setPosition(player->shot_spr, player->shot_rect.x, player->shot_rect.y);
}

void fireShot(void)
{
    player->shot_rect.x = player->player_rect.x + 12;
    player->shot_rect.y = player->player_rect.y;
}

bool isShotOutOfBounds(void)
{
    return player->shot_rect.y <= -10;
}

void destructPlayer(void)
{
    SPR_releaseSprite(player->shot_spr);    
    SPR_releaseSprite(player->player_spr);        
    SPR_releaseSprite(player->hit_box_spr);
        
    MEM_free(player);
}

void moveLeft(void)
{
    player->velocity.x = -player->speed;
    SPR_setAnim(player->player_spr, 1);
}

void moveRight(void)
{
    player->velocity.x = player->speed;
    SPR_setAnim(player->player_spr, 2);
}

void moveUp(void)
{
    player->velocity.y = -player->speed;
}

void moveDown(void)
{
    player->velocity.y = player->speed;
}

void haltX(void)
{
    player->velocity.x = 0;
    SPR_setAnim(player->player_spr, 0);
}

void haltY(void)
{
    player->velocity.y = 0;
}

void enableShotMovement(void)
{
    player->shot_velocity.y = -player->shot_speed;
}

void disableShotMovement(void)
{
    player->shot_velocity.y = 0;
}

Rectangle_t getPlayerRect(void)
{
    return player->player_rect;
}

Rectangle_t getShotRect(void)
{
    return player->shot_rect;
}

Rectangle_t getHitboxRect(void)
{
    return player->hitbox_rect;
}