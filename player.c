#include "player.h"

Player_t* player = NULL;

void ResetPlayer()
{
    player->speed = 3;
    player->shotSpeed = 6;
    
    player->player_rect.height =  32;
    player->player_rect.width =  32;
    player->player_rect.x =  144;
    player->player_rect.y =  160;

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

Player_t* GetInitialisedPlayer()
{
    if(player != NULL)
    {
        return player;
    }

    player = (Player_t*) MEM_alloc(sizeof(Player_t)); 

    ResetPlayer();

    player->hit_box_spr = SPR_addSprite(&hitBox, player->hitbox_rect.x, player->hitbox_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE)); 
    player->player_spr = SPR_addSprite(&paddle, player->player_rect.x, player->player_rect.y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    player->shot_spr = SPR_addSprite(&imgball, player->shot_rect.x, player->shot_rect.y, TILE_ATTR(PAL2, 2, FALSE, FALSE));
    return player;
}

void DestructPlayer()
{
    MEM_free(player);
}
