#include "background.h"
#include "game_update_observable.h"
static u8 scrollSpeed = 4;

void BCK_update(void)
{
    static u8 bg_scroll_speed = 0;

    bg_scroll_speed -= scrollSpeed;

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

void BCK_draw_starfield(void)
{
    scrollSpeed = 5;
    VDP_clearPlan(PLAN_A, 0);
    VDP_drawImageEx(PLAN_A, &starfield, TILE_ATTR_FULL(PAL3, 0, 1, 0, 1), 0, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &starfield, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 16, 0, CPU);
    VDP_drawImageEx(PLAN_A, &starfield, TILE_ATTR_FULL(PAL3, 0, 0, 1, 1), 16, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &starfield, TILE_ATTR_FULL(PAL3, 0, 1, 0, 1), 16, 16, 0, CPU);
    VDP_drawImageEx(PLAN_A, &starfield, TILE_ATTR_FULL(PAL3, 0, 1, 1, 1), 32, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &starfield, TILE_ATTR_FULL(PAL3, 0, 1, 0, 1), 32, 16, 0, CPU);
}

void BCK_draw_darkSky(void)
{
    scrollSpeed = 4;
    VDP_clearPlan(PLAN_A, 0);
    VDP_drawImageEx(PLAN_A, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 16, 0, CPU);
    VDP_drawImageEx(PLAN_A, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 16, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 16, 16, 0, CPU);
    VDP_drawImageEx(PLAN_A, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 32, 0, 0, CPU);
    VDP_drawImageEx(PLAN_A, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 32, 16, 0, CPU);
}

void BCK_draw_title_screen(void)
{
    scrollSpeed = 0;
    VDP_clearPlan(PLAN_A, 0);
    VDP_drawImageEx(PLAN_A, &introImage, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0, 0, CPU);
}

void BCK_draw_intro_screen(void)
{
    scrollSpeed = 0;
    VDP_clearPlan(PLAN_A, 0);
    VDP_drawImageEx(PLAN_A, &flump, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0, 0, CPU);
}

void BCK_init(void)
{
    addTickFunc(BCK_update, true);
}