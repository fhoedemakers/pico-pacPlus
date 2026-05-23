#ifndef VMACHINE_H
#define VMACHINE_H

#include "types.h"
#include <stdint.h>
#ifndef __O2EM_SDL__
#include "allegro.h"
#else
#include "o2em_sdl.h"
#endif

#define LINECNT 21
#define MAXLINES 500
#define MAXSNAP 50

#define VBLCLK 5493
#define EVBLCLK_NTSC 5964
#define EVBLCLK_PAL 7259

#define FPS_NTSC 60
#define FPS_PAL 50
#define WANT_FPS_PAL 1
#define WANT_FPS_NTSC 0

extern Byte dbstick1, dbstick2;
extern int last_line;

extern int evblclk;

extern int master_clk;		/* Master clock */
extern int int_clk;		/* counter for length of /INT pulses for JNI */
extern int h_clk;   /* horizontal clock */
extern Byte coltab[256];
extern int mstate;

extern Byte rom_table[8][4096];
extern Byte intRAM[];
extern Byte extRAM[];
extern Byte extROM[];
extern Byte VDCwrite[256];
extern Byte ColorVector[MAXLINES];
extern Byte AudioVector[MAXLINES];
extern Byte *rom;
extern Byte *megarom;

extern int frame;
int key2[KEY_MAX];
extern int key2vcnt;
extern unsigned long clk_counter;

int joykeystab[KEY_MAX];

extern int enahirq;
extern int pendirq;
extern int useforen;
extern long regionoff;
extern int sproff;
extern int tweakedaudio;

SAMPLE *voices[9][128];

Byte read_P2();
int snapline(int pos, Byte reg, int t);
void ext_write(Byte dat, ADDRESS adr);
Byte ext_read(ADDRESS adr);
void handle_vbl();
void handle_evbl();
void handle_evbll();
Byte in_bus();
void write_p1(Byte d);
Byte read_t1();
void init_system();
void init_roms();
void init_rams();
void run();
int savestate(char *filename);
int loadstate(char *filename);
void o2em_clean_quit();

/*#pragma pack(8)*/
extern struct resource {
	int bank;
	int debug;
	int stick[2];
	int sticknumber[2];
	int limit;
	int sound_en;
	int speed;
	int wsize;
	int fullscreen;
	int scanlines;
	int voice;
	int svolume;
	int vvolume;
	int exrom;
	int three_k;
	int filter;
	int euro;
	int openb;
	int megaxrom;
	int vpp;
	int bios;
	uint32_t crc;
	char *window_title;
	char *scshot;
	int scoretype;
	int scoreaddress;
	int default_highscore;
	int breakpoint;
	char statefile[MAXC];
	char romdir[MAXC];
	char biosdir[MAXC];
	char bios_filename[MAXC];
	char voice_path[MAXC];
	int need_bios;
	int force_bios;
	int show_fps;
	int handleevbl;
	int handleevbll;
	int allowvdcalways;
	int c7420;
	int disableJoyDefault;
} app_data;


#endif  /* VMACHINE_H */

