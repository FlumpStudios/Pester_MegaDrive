#include "background.h"
#include "game_update_observable.h"

void BCK_update(void)
{
    static f16 bg_scroll_speed = 0;

    bg_scroll_speed -= 4;

    if (bg_scroll_speed <= -250)
    {
        bg_scroll_speed = 0;
    }

    if (bg_scroll_speed <= -250)
    {
        bg_scroll_speed = 0;
    }

    VDP_setVerticalScroll(PLAN_A, bg_scroll_speed);
}

void BCK_init_starfield(void)
{
    addTickFunc(BCK_update, true);
    VDP_clearPlan(PLAN_A, 0);
    VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 1, 0, 1), 0, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 1, 1), 0, 16, 0, CPU);
    VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 16, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 1, 1, 1), 16, 16, 0, CPU);
    VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 32, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &tile, TILE_ATTR_FULL(PAL1, 0, 1, 0, 1), 32, 16, 0, CPU);
}

void BCK_init_title_screen(void)
{
    VDP_clearPlan(PLAN_A, 0);
    VDP_drawImageEx(PLAN_A, &introImage, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0, 0, CPU);
}