#include "vmachine.h"

#ifndef __O2EM_ROMS__
#define __O2EM_ROMS__
int load_cart(char *file, struct resource *app_data);
int search_for_rom(char *pathx, char *rom_searched, char *full_path_to_rom);
int identify_rom(char *filename);
int identify_all_roms(char *pathx);
#endif

