#include "audio.h"

#define SFX_PLAYER_FIRE 64
#define SFX_EXPLOSION 65
#define SFX_HIT 66
#define SFX_START 67
#define SFX_PLAYER_DEATH 68
#define SFX_WAVE_COMPLETE 69
#define SFX_GAME_OVER 70
#define SFX_ALARM 71
#define SFX_LOST_CHAIN 72

void AUD_init(void)
{    
    XGM_setPCM(SFX_PLAYER_FIRE, sfx_player_fire, sizeof(sfx_player_fire));
    XGM_setPCM(SFX_EXPLOSION, sfx_explosion, sizeof(sfx_explosion));
    XGM_setPCM(SFX_HIT, sfX_hit, sizeof(sfX_hit));
    XGM_setPCM(SFX_START, sfX_start, sizeof(sfX_start));
    XGM_setPCM(SFX_PLAYER_DEATH, sfX_player_death, sizeof(sfX_player_death));
    XGM_setPCM(SFX_WAVE_COMPLETE, sfX_wave_complete, sizeof(sfX_wave_complete));
    XGM_setPCM(SFX_GAME_OVER, sfX_game_over, sizeof(sfX_game_over));
    XGM_setPCM(SFX_ALARM, sfx_alarm, sizeof(sfx_alarm));
    XGM_setPCM(SFX_LOST_CHAIN, sfx_loseChain, sizeof(sfx_loseChain));
}

void AUD_play_player_shot(void)
{
    XGM_startPlayPCM(SFX_PLAYER_FIRE,2,SOUND_PCM_CH2);
}

void AUD_play_explosion(void)
{
   XGM_startPlayPCM(SFX_EXPLOSION,2,SOUND_PCM_CH3);
}

void AUD_play_hit(void)
{
    XGM_startPlayPCM(SFX_HIT,1,SOUND_PCM_CH3);
}

void AUD_play_start(void)
{
    XGM_startPlayPCM(SFX_START,1,SOUND_PCM_CH2);
}

void AUD_play_player_death(void)
{
    XGM_startPlayPCM(SFX_PLAYER_DEATH,3,SOUND_PCM_CH2);
}

void AUD_play_wave_complete(void)
{
    XGM_startPlayPCM(SFX_WAVE_COMPLETE,3,SOUND_PCM_CH3);
}

void AUD_play_game_over(void)
{
    XGM_startPlayPCM(SFX_GAME_OVER,4,SOUND_PCM_CH3);
}

void AUD_play_alarm(void)
{
    XGM_startPlayPCM(SFX_ALARM,5,SOUND_PCM_CH3);
}

void AUD_play_lost_chain(void)
{
    XGM_startPlayPCM(SFX_LOST_CHAIN,5,SOUND_PCM_CH3);
}

void AUD_play_menu_music(void)
{
	XGM_setLoopNumber(-1);
	XGM_startPlay(menuMusic);
}

void AUD_play_level1_music(void)
{
	XGM_setLoopNumber(-1);
	XGM_startPlay(track1);
}