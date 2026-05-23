#ifndef __O2EM_PICO_CALLBACKS_H__
#define __O2EM_PICO_CALLBACKS_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void o2em_render_frame(Byte *vscreen, Byte *col, int width, int height, const unsigned short *palette_lut, int vpp_enabled);
void o2em_sound_output(unsigned char *buffer, int len);
void o2em_poll_input(void);

#ifdef __cplusplus
}
#endif

#endif
