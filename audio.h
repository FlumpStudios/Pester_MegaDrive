#ifndef AUDIO_H
#define AUDIO_H
#include "common.h"
void AUD_init(void);
void AUD_play_player_shot(void);
void AUD_play_explosion(void);
void AUD_play_hit(void);
void AUD_play_start(void);
void AUD_play_player_death(void);
void AUD_play_wave_complete(void);
void AUD_play_game_over(void);
void AUD_play_alarm(void);
void AUD_play_menu_music(void);
void AUD_play_level1_music(void);
void AUD_play_lost_chain(void);
#endif