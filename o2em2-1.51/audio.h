#ifndef __AUDIO_H
#define __AUDIO_H

void update_audio();
int o2em_init_audio();
void close_audio();
void init_sound_stream();
void mute_audio();

extern int sound_IRQ;
extern FILE *sndlog;

#endif

