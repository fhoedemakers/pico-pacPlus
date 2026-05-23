#ifndef __O2EM_PICO_H__
#define __O2EM_PICO_H__

#include "types.h"
#include "pico/stdlib.h"

#ifndef HSTX
#if GPIOHSTXD0 && GPIOHSTXD1 && GPIOHSTXD2 && GPIOHSTXCK && PICO_RP2350
#define HSTX 1
#else
#define HSTX 0
#endif
#endif

#define KEY_A       0
#define KEY_B       1
#define KEY_C       2
#define KEY_D       3
#define KEY_E       4
#define KEY_F       5
#define KEY_G       6
#define KEY_H       7
#define KEY_I       8
#define KEY_J       9
#define KEY_K       10
#define KEY_L       11
#define KEY_M       12
#define KEY_N       13
#define KEY_O       14
#define KEY_P       15
#define KEY_Q       16
#define KEY_R       17
#define KEY_S       18
#define KEY_T       19
#define KEY_U       20
#define KEY_V       21
#define KEY_W       22
#define KEY_X       23
#define KEY_Y       24
#define KEY_Z       25

#define KEY_0       26
#define KEY_1       27
#define KEY_2       28
#define KEY_3       29
#define KEY_4       30
#define KEY_5       31
#define KEY_6       32
#define KEY_7       33
#define KEY_8       34
#define KEY_9       35

#define KEY_0_PAD   36
#define KEY_1_PAD   37
#define KEY_2_PAD   38
#define KEY_3_PAD   39
#define KEY_4_PAD   40
#define KEY_5_PAD   41
#define KEY_6_PAD   42
#define KEY_7_PAD   43
#define KEY_8_PAD   44
#define KEY_9_PAD   45

#define KEY_F1      46
#define KEY_F2      47
#define KEY_F3      48
#define KEY_F4      49
#define KEY_F5      50
#define KEY_F6      51
#define KEY_F7      52
#define KEY_F8      53
#define KEY_F9      54
#define KEY_F10     55
#define KEY_F11     56
#define KEY_F12     57

#define KEY_LEFT    58
#define KEY_RIGHT   59
#define KEY_UP      60
#define KEY_DOWN    61
#define KEY_ENTER   62
#define KEY_SPACE   63
#define KEY_ESC     64
#define KEY_DEL     65
#define KEY_SLASH   66
#define KEY_STOP    67
#define KEY_MINUS   68
#define KEY_ASTERISK 69
#define KEY_SLASH_PAD 70
#define KEY_PLUS_PAD 71
#define KEY_EQUALS  72
#define KEY_BACKSPACE 73
#define KEY_TAB     74
#define KEY_OPENBRACE 75
#define KEY_CLOSEBRACE 76
#define KEY_COLON   77
#define KEY_QUOTE   78
#define KEY_BACKSLASH 79
#define KEY_BACKSLASH2 80
#define KEY_COMMA   81
#define KEY_INSERT  82
#define KEY_HOME    83
#define KEY_END     84
#define KEY_PGUP    85
#define KEY_PGDN    86
#define KEY_MINUS_PAD 87
#define KEY_DEL_PAD 88
#define KEY_ENTER_PAD 89
#define KEY_PRTSCR  90
#define KEY_PAUSE   91
#define KEY_LSHIFT  92
#define KEY_RSHIFT  93
#define KEY_LCONTROL 94
#define KEY_RCONTROL 95
#define KEY_ALT     96
#define KEY_ALTGR   97
#define KEY_LWIN    98
#define KEY_RWIN    99
#define KEY_MENU    100
#define KEY_SCRLOCK 101
#define KEY_NUMLOCK 102
#define KEY_TILDE   103
#define KEY_ABNT_C1 104
#define KEY_YEN     105
#define KEY_KANA    106
#define KEY_AT      107
#define KEY_CIRCUMFLEX 108
#define KEY_COLON2  109
#define KEY_KANJI   110
#define KEY_TIGHT   111
#define KEY_CAPSLOCK 112

#define KEY_MAX 128

#define KB_CAPSLOCK_FLAG 1

#define JOY_TYPE_AUTODETECT 0
#define GFX_AUTODETECT_FULLSCREEN 1
#define GFX_AUTODETECT_WINDOWED 2
#define GFX_AUTODETECT 3
#define GFX_TEXT 4
#define SWITCH_BACKGROUND 3
#define SWITCH_PAUSE 1
#define SYSTEM_AUTODETECT 0

#define END_OF_MAIN()

typedef int AUDIOSTREAM;
typedef int SAMPLE;
typedef int FONT;
typedef void BITMAP;
typedef unsigned char PALETTE[256][3];

struct Axis {
    int d1, d2;
};

struct Stick {
    struct Axis axis[2];
};

struct Button {
    int b;
};

struct Joystick {
    struct Stick stick[4];
    struct Button button[4];
};

extern struct Joystick joy[2];
extern unsigned char key[KEY_MAX];
extern char allegro_error[];
extern int key_shifts;
extern int num_joysticks;

#define SCREEN_W 320
#define SCREEN_H 240

#define allegro_init() (0)
#define install_keyboard() (0)
#define install_timer() (0)
#define install_joystick(t) (0)
#define remove_keyboard()
#define remove_sound()
#define poll_keyboard() (0)
#define keyboard_needs_poll() (0)
#define set_display_switch_mode(m) (0)
#define set_color_depth(d)
#define set_gfx_mode(c,w,h,vw,vh) (0)
#define set_palette(p)
#define get_palette(p)
#define check_palette(p) (0)
#define set_window_title(n)
#define acquire_screen()
#define release_screen()
#define create_bitmap(w,h) ((BITMAP*)0)
#define clear(b)
#define set_volume(d,m)
#define install_sound(d,m,c) (0)
#define rest(ms) sleep_ms(ms)
#define textout_centre_ex(b,f,s,x,y,c,bg)
#define textprintf_ex(b,f,x,y,c,bg,fmt,...)
#define stretch_blit(s,d,sx,sy,sw,sh,dx,dy,dw,dh)
#define rectfill(b,x1,y1,x2,y2,c)
#define line(...)

#define get_audio_stream_buffer(s) ((void*)0)
#define free_audio_stream_buffer(s)
#define stop_audio_stream(s)
#define play_audio_stream(l,b,s,f,v,p) ((AUDIOSTREAM*)0)
#define voice_get_position(v) (-1)
#define allocate_voice(s) (-1)
#define voice_start(v)
#define voice_stop(v)
#define reallocate_voice(v,s)
#define load_sample(f) ((SAMPLE*)0)
#define destroy_sample(s)
#define voice_set_volume(v,vol)

extern void *font;
extern BITMAP *screen;

char *strlwr(char *str);
char *strupr(char *str);

#endif
