#define BIOS_ODYSSEY2 1
#define BIOS_G7400 2
#define BIOS_C52 4
#define BIOS_JOPAC 8

#define CRC_ODYSSEY2 0x8016A315
#define CRC_G7400 0xE20A9F41
#define CRC_C52 0xA318E8D6
#define CRC_JOPAC 0x11647CA5

#include "vmachine.h"

int suck_bios();
int identify_bios(char *biossux, struct resource* app_data);
int load_bios(const char *biosname, unsigned char *rom_table[4096], struct resource *app_data);
int search_for_bios(char *pathx, char *bios_found, int bios_type);

