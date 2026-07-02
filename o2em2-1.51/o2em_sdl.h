#ifdef __O2EM_SDL__

#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>

#ifndef __O2EM_SDL_H__
#define __O2EM_SDL_H__

#define KEY_LEFT SDLK_LEFT
#define KEY_TIGHT SDLK_RIGHT
#define KEY_UP SDLK_UP
#define KEY_DOWN SDLK_DOWN
#define KEY_ENTER SDLK_RETURN
#define KEY_SPACE SDLK_SPACE

#define KEY_DEL SDLK_DELETE
#define KEY_SLASH SDLK_SLASH
#define KEY_STOP SDLK_END
#define KEY_MINUS SDLK_MINUS
#define KEY_ASTERISK SDLK_ASTERISK
#define KEY_SLASH_PAD SDLK_KP_DIVIDE
#define KEY_PLUS_PAD SDLK_KP_PLUS
#define KEY_EQUALS SDLK_EQUALS
#define KEY_ESC SDLK_ESCAPE


#define KEY_0_PAD SDLK_KP0
#define KEY_1_PAD SDLK_KP1
#define KEY_2_PAD SDLK_KP2
#define KEY_3_PAD SDLK_KP3
#define KEY_4_PAD SDLK_KP4
#define KEY_5_PAD SDLK_KP5
#define KEY_6_PAD SDLK_KP6
#define KEY_7_PAD SDLK_KP7
#define KEY_8_PAD SDLK_KP8
#define KEY_9_PAD SDLK_KP9

#define KEY_0 SDLK_0
#define KEY_1 SDLK_1
#define KEY_2 SDLK_2
#define KEY_3 SDLK_3
#define KEY_4 SDLK_4
#define KEY_5 SDLK_5
#define KEY_6 SDLK_6
#define KEY_7 SDLK_7
#define KEY_8 SDLK_8
#define KEY_9 SDLK_9

#define KEY_A SDLK_a
#define KEY_B SDLK_b
#define KEY_C SDLK_c
#define KEY_D SDLK_d
#define KEY_E SDLK_e
#define KEY_F SDLK_f
#define KEY_G SDLK_g
#define KEY_H SDLK_h
#define KEY_I SDLK_i
#define KEY_J SDLK_j
#define KEY_K SDLK_k
#define KEY_L SDLK_l
#define KEY_M SDLK_m
#define KEY_N SDLK_n
#define KEY_O SDLK_o
#define KEY_P SDLK_p
#define KEY_Q SDLK_q
#define KEY_R SDLK_r
#define KEY_S SDLK_s
#define KEY_T SDLK_t
#define KEY_U SDLK_u
#define KEY_V SDLK_v
#define KEY_W SDLK_w
#define KEY_X SDLK_x
#define KEY_Y SDLK_y
#define KEY_Z SDLK_z

#define KEY_F1 SDLK_F1
#define KEY_F2 SDLK_F2
#define KEY_F3 SDLK_F3
#define KEY_F4 SDLK_F4
#define KEY_F5 SDLK_F5
#define KEY_F6 SDLK_F6
#define KEY_F7 SDLK_F7
#define KEY_F8 SDLK_F8
#define KEY_F9 SDLK_F9
#define KEY_F10 SDLK_F10
#define KEY_F11 SDLK_F11
#define KEY_F12 SDLK_F12

#define KEY_BACKSPACE SDLK_BACKSPACE
#define KEY_TAB SDLK_TAB
#define KEY_OPENBRACE SDLK_LEFTBRACKET
#define KEY_CLOSEBRACE SDLK_RIGHTBRACKET
#define KEY_COLON SDLK_COLON
#define KEY_QUOTE SDLK_QUOTE
#define KEY_BACKSLASH SDLK_BACKSLASH
#define KEY_BACKSLASH2 SDLK_BACKSLASH
#define KEY_COMMA SDLK_COMMA
#define KEY_INSERT SDLK_INSERT
#define KEY_HOME SDLK_HOME
#define KEY_END SDLK_END
#define KEY_PGUP SDLK_PAGEUP
#define KEY_PGDN SDLK_PAGEDOWN
#define KEY_RIGHT SDLK_RIGHT
#define KEY_MINUS_PAD SDLK_KP_MINUS
#define KEY_DEL_PAD SDLK_KP_PERIOD
#define KEY_ENTER_PAD SDLK_KP_ENTER
#define KEY_PRTSCR SDLK_PRINT
#define KEY_PAUSE SDLK_PAUSE
#define KEY_ABNT_C1 0
#define KEY_YEN 0
#define KEY_KANA 0
#define KEY_AT SDLK_AT
#define KEY_CIRCUMFLEX 0
#define KEY_COLON2 0
#define KEY_KANJI 0
#define KEY_LSHIFT SDLK_LSHIFT
#define KEY_RSHIFT SDLK_RSHIFT
#define KEY_LCONTROL SDLK_LCTRL
#define KEY_RCONTROL SDLK_RCTRL
#define KEY_ALT SDLK_RALT
#define KEY_ALTGR SDLK_MODE
#define KEY_LWIN SDLK_LSUPER
#define KEY_RWIN SDLK_RSUPER
#define KEY_MENU SDLK_MENU
#define KEY_SCRLOCK SDLK_SCROLLOCK
#define KEY_NUMLOCK SDLK_NUMLOCK


#define KEY_TILDE 0

#define KB_CAPSLOCK_FLAG 1

#define JOY_TYPE_AUTODETECT 0

#define GFX_AUTODETECT_FULLSCREEN 1
#define GFX_AUTODETECT_WINDOWED 2
#define GFX_AUTODETECT 3
#define GFX_TEXT 4
#define SWITCH_BACKGROUND     3

int SCREEN_W;
int SCREEN_H;

#define SWITCH_PAUSE 1
#define SYSTEM_AUTODETECT  0
#define END_OF_MAIN()/*TODO REAL FUNCTION*/

typedef int AUDIOSTREAM;
typedef int SAMPLE;
typedef int FONT;
typedef SDL_Surface BITMAP;

extern SDL_Color colors[256];
struct Axis
{
	int d1, d2;
};

struct Stick
{
	struct Axis axis[2];
};
struct Button
{
	int b;
};
struct Joystick
{
	struct Stick stick[4];
	struct Button button[4];
};
struct Joystick joy[2];

/*TODO downcase*/
int DISPLAY_DEPTH;
void *font;

#define KEY_MAX SDLK_LAST
Uint8 *key;

char allegro_error[1024];
BITMAP *screen;

int key_shifts; /* TODO used in vmachine.c but never init */

int num_joysticks; /* TODO used in vmachine.c but never init */


/* values of allegro*/
#define TRUE -1
#define FALSE 0


/* keyboard function*/
int install_keyboard();
int poll_keyboard();
int keyboard_needs_poll();
void remove_keyboard();

int install_joystick(int joytype);

/* gfx */
int set_display_switch_mode(int mode);
BITMAP *create_bitmap(int w, int h);
void clear(BITMAP *bitmap);
void set_color_depth(int depth);
int set_gfx_mode(int card, int w, int h, int v_w, int v_h);
void set_palette(SDL_Color *p);
void get_palette(SDL_Color *p);
int check_palette(SDL_Color *p);
void set_window_title(char * name);
void acquire_screen();
void release_screen();
unsigned char *get_raw_pixel_line(SDL_Surface *pSurface, int y);
unsigned char *get_raw_pixel(SDL_Surface *pSurface, int x, int y);


void textout_centre_ex(BITMAP *bmp, void *f, const char *s, int x, int y, int color, int bg);
void textprintf_ex(BITMAP *bmp, void *f, int x, int y, int color, int bg, const char *fmt, ...);

void stretch_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int source_width, int source_height,
	int dest_x, int dest_y, int dest_width, int dest_height);

void rectfill(BITMAP *bmp,int x1, int y1, int x2, int y2, int color);
void line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);


/* misc function*/
void allegro_init();
char *strlwr(char *str);
char *strupr(char *str);
int install_timer();
void rest(unsigned int time);

/* sound function */
typedef enum { DIGI_AUTODETECT } DigitalType;
typedef enum { MIDI_NONE } MidiType;
void *get_audio_stream_buffer(AUDIOSTREAM *stream);
void free_audio_stream_buffer(AUDIOSTREAM *stream);
void stop_audio_stream(AUDIOSTREAM *stream);
int install_sound(int digi, int midi, const char *cfg_path);
void remove_sound();
void set_volume(int digi_volume, int midi_volume);
AUDIOSTREAM *play_audio_stream(int len, int bits, int stereo, int freq, int vol, int pan);
int voice_get_position(int voice);
int allocate_voice(const SAMPLE *spl);
void voice_start(int voice_num);
void voice_stop(int voice);
void reallocate_voice(int voice, const SAMPLE *spl);
SAMPLE *load_sample(const char *filename);
void destroy_sample(SAMPLE *spl);
void set_volume(int digi_volume, int midi_volume);
void voice_set_volume(int voice, int volume);



#endif
#endif

