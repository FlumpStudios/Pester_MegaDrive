#include "background.h"
#include "game_update_observable.h"

static u8 frameskip = 2;
static u8 frame = 0;
static u8 scrollSpeed = 1;
static u8 bg_scroll_speed = 255;

void BCK_update(void)
{
    frame++;
    if (frame > frameskip && scrollSpeed > 0)
    {
        frame = 0;
        bg_scroll_speed -= scrollSpeed;

        if (bg_scroll_speed <= 0)
        {
            bg_scroll_speed = 255;
        }

       VDP_setVerticalScroll(PLAN_B, bg_scroll_speed);
    }
}

void BCK_draw_starfield(void)
{
    scrollSpeed = 1;
    frameskip = 2;
    VDP_clearPlan(PLAN_B, 0);
    VDP_drawImageEx(PLAN_B, &starfield, TILE_ATTR_FULL(PAL3, 0, 1, 0, 1), 0, 0, 0, CPU);
    VDP_drawImageEx(PLAN_B, &starfield, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 16, 0, CPU);
    VDP_drawImageEx(PLAN_B, &starfield, TILE_ATTR_FULL(PAL3, 0, 0, 1, 1), 16, 0, 0, CPU);
    VDP_drawImageEx(PLAN_B, &starfield, TILE_ATTR_FULL(PAL3, 0, 1, 0, 1), 16, 16, 0, CPU);
    VDP_drawImageEx(PLAN_B, &starfield, TILE_ATTR_FULL(PAL3, 0, 1, 1, 1), 32, 0, 0, CPU);
    VDP_drawImageEx(PLAN_B, &starfield, TILE_ATTR_FULL(PAL3, 0, 1, 0, 1), 32, 16, 0, CPU);
}

void BCK_draw_moon(void)
{
    scrollSpeed = 1;
    frameskip = 2;
    VDP_clearPlan(PLAN_B, 0);
    VDP_drawImageEx(PLAN_B, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0, 0, CPU);
    VDP_drawImageEx(PLAN_B, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 16, 0, CPU);
    VDP_drawImageEx(PLAN_B, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 16, 0, 0, CPU);
    VDP_drawImageEx(PLAN_B, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 16, 16, 0, CPU);
    VDP_drawImageEx(PLAN_B, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 32, 0, 0, CPU);
    VDP_drawImageEx(PLAN_B, &moon, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 32, 16, 0, CPU);
}

void BCK_draw_title_screen(void)
{
    scrollSpeed = 0;
    VDP_clearPlan(PLAN_B, 0);
    VDP_setVerticalScroll(PLAN_B, 0);
    VDP_drawImageEx(PLAN_B, &introImage, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0, 0, CPU);
}

void BCK_draw_intro_screen(void)
{
    scrollSpeed = 0;
    VDP_clearPlan(PLAN_B, 0);
    VDP_drawImageEx(PLAN_B, &flump, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0, 0, CPU);
}

void BCK_init(void)
{
    static bool tickAdded = false;
    if (!tickAdded)
    {
        addTickFunc(BCK_update, true);
        tickAdded = true;
    }
}