#ifndef __VDC_H
#define __VDC_H

#ifndef __O2EM_SDL__
#include <allegro.h>
#else
#include "o2em_sdl.h"
#endif

#define BMPW 340
#define BMPH 250
#define WNDW 320
#define WNDH 240
extern Byte coltab[];
extern long clip_low;
extern long clip_high;
extern int show_fps;

BITMAP *bmp;
BITMAP *bmpcache;
BITMAP *vppbmp;
Byte *col;

int init_display();
void draw_display();
void set_textmode();
void draw_region();
void finish_display();
void close_display();
void grmode();
void display_bg();
void display_msg(char *msg, int waits);
void clear_collision();
void clearscr();
void about();
void help();
void create_cmap();

#endif

