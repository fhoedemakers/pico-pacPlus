#ifndef __KEYBOARD_H
#define __KEYBOARD_H

void Set_Old_Int9();
int o2em_init_keyboard();
void handle_key();
void set_joykeys(int joy, int up, int down, int left, int right, int fire);
void set_systemkeys(int k_quit, int k_pause, int k_debug, int k_reset, int k_screencap, int k_save, int k_load, int k_inject,
		int k_swap, int k_load_c7420, int k_save_c7420, int k_copy_c7420);
void set_defjoykeys(int joy, int sc);
void set_defsystemkeys();
Byte keyjoy(int jn);

extern	Byte new_int;
extern int NeedsPoll;
extern Byte key_done;
extern Byte key_debug;
extern int joykeys[2][5];
int syskeys[12];
/*int syskeys[12] = {0,0,0,0,0,0,0,0,0,0,0,0};*/

struct keyb{
	int keybcode;
	char *keybname;
};

extern struct keyb keybtab[];

#define KEY_DELAY 5

#endif

