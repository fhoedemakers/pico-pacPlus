#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "o2em_pico.h"
#include "o2em2.h"

unsigned char key[KEY_MAX];
struct Joystick joy[2];
char allegro_error[64];
int key_shifts = 0;
int num_joysticks = 0;
void *font = 0;
BITMAP *screen = 0;

void o2em_clean_quit(int exitcode) {
    printf("o2em_clean_quit(%d)\n", exitcode);
    while(1) { __asm volatile("wfi"); }
}

char *strlwr(char *str) {
    for (char *p = str; *p; p++)
        *p = tolower((unsigned char)*p);
    return str;
}

char *strupr(char *str) {
    for (char *p = str; *p; p++)
        *p = toupper((unsigned char)*p);
    return str;
}
