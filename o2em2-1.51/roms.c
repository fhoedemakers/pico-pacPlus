/*
 *   O2EM2 Free Odyssey2 / Videopac+ Emulator
 *
 *   Under development by LABBE Corentin http://o2em2.sourceforge.net
 *
 *
 *
 *   Bios search and load functions
 */


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include "o2em2.h"
#include "crc32.h"
#include "roms.h"
#include "bios.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*===========================================================================*/
/*===========================================================================*/
/* TODO missing open with full path and without .bin and with roms/xxxx */
int search_for_rom(char *pathx, char *rom_searched, char *full_path_to_rom)
{
	DIR *dir_p;
	struct dirent *dir_entry_p;
	int fd;

	if (pathx == NULL || full_path_to_rom == NULL || rom_searched == NULL) {
		fprintf(stderr, "bad usage of %s\n", __func__);
		return O2EM_FAILURE;
	}
	dir_p = opendir(pathx);
	if (dir_p == NULL) {
		fprintf(stderr, "dir '%s' not found !\n", pathx);
		return O2EM_FAILURE;
	}
	#ifdef __O2EM_VERBOSE__
	printf("Check directory %s\n", pathx);
	#endif
	while (0 != (dir_entry_p = readdir(dir_p))) {
		if (strcmp(dir_entry_p->d_name, ".") != 0 && strcmp(dir_entry_p->d_name, "..") != 0) {
			memset(full_path_to_rom, 0, MAXC);
			snprintf(full_path_to_rom, MAXC, "%s/%s", pathx, dir_entry_p->d_name);
			#ifdef __O2EM_VERBOSE__
			printf("  Check %s\n", dir_entry_p->d_name);
			#endif
			if (strcmp(dir_entry_p->d_name, rom_searched) == 0) {
				#ifdef __O2EM_VERBOSE__
				printf("    ROM FOUND\n");
				#endif
				closedir(dir_p);
				return O2EM_SUCCESS;
			}
		}
	}
	closedir(dir_p);
	/* since we havent found the rom, perhaps it is full path */
	fd = open(rom_searched, O_RDONLY);
	if (fd >= 0) {
		strncpy(full_path_to_rom, rom_searched, MAXC);
		close(fd);
#ifdef __O2EM_VERBOSE__
		printf("    ROM FOUND with full path\n");
#endif
		return O2EM_SUCCESS;
	}
	return O2EM_FAILURE;
}

/*===========================================================================*/
/*===========================================================================*/
/* TODO return of functions */
long filesize(FILE *stream)
{
	long curpos, length;
	if (stream == NULL) {
		fprintf(stderr, "%s stream is NULL\n", __func__);
		return -1;
	}
	curpos = ftell(stream);
	if (curpos == -1) {
		fprintf(stderr, "%s Error %s\n", __func__, strerror(errno));
		return -1;
	}
	fseek(stream, 0L, SEEK_END);
	length = ftell(stream);
	fseek(stream, curpos, SEEK_SET);
	return length;
}

/*===========================================================================*/
/*===========================================================================*/
int load_cart(char *file, struct resource *app_data)
{
	FILE *fn;
	long l;
	int i, nb;
	size_t nbread;

	if (app_data == NULL) {
		fprintf(stderr, "%s app_data NULL\n", __func__);
		return O2EM_FAILURE;
	}
	#ifdef __O2EM_VERBOSE__
	printf("BEGIN of %s() with %s\n", __func__, file);
	#endif
	identify_rom(file);
	goto plop;
	app_data->crc = crc32_file(file);
	switch (app_data->crc) {
		case 0x4EDA4917:/* 01 */
		case 0xABE368BF:/* 14 */
			app_data->need_bios = BIOS_ODYSSEY2;
			break;
		case 0xAFB23F89: /* Musician */
			app_data->exrom = 1;
			printf("%s QUIRK exom = 1\n", __func__);
			break;
		case 0x3BFEF56B: /* Four in 1 Row! */
			app_data->exrom = 1;
			break;
		case 0x9B5E9356: /* Four in 1 Row! (french) */
			app_data->exrom = 1;
			break;
		case 0xb4735e6b: /* 34+ Sattelite Attack */
			app_data->need_bios = BIOS_ODYSSEY2 | BIOS_G7400 | BIOS_C52 | BIOS_JOPAC;/* tested BIOS_G7400*/
			break;
		case 0x764894A1: /* 51 */
			app_data->need_bios = BIOS_ODYSSEY2 | BIOS_G7400 | BIOS_C52 | BIOS_JOPAC;/* tested BIOS_G7400*/
			break;
		default:
			printf("%s unknown CRC %x falling back to BIOS_ODYSSEY2\n", __func__, app_data->crc);
			app_data->need_bios = BIOS_ODYSSEY2;
			break;
	}
	plop:
	/* TODO what is theses CRC ? */
	if (((app_data->crc == 0x975AB8DA) || (app_data->crc == 0xE246A812)) && (!app_data->debug)) {
		fprintf(stderr, "Error: file %s is an incomplete ROM dump\n", file);
		return O2EM_FAILURE;
	}
	fn = fopen(file, "rb");
	if (fn == NULL) {
		fprintf(stderr, "Error loading %s\n", file);
		return O2EM_FAILURE;
	}
	printf("Loading: \"%s\"  Size: ", file);
	l = filesize(fn);

	if ((l % 1024) != 0) {
		fprintf(stderr, "Error: file %s is an invalid ROM dump\n", file);
		fclose(fn);
		return O2EM_FAILURE;
	}

	/* special MegaCART design by Soeren Gust */
	if ((l == 32768) || (l == 65536) || (l == 131072) || (l == 262144) || (l == 524288) || (l == 1048576)) {
		app_data->megaxrom = 1;
		app_data->bank = 1;
		megarom = malloc(1048576);/* 1024 * 1024 */
		#ifdef __O2EM_MEM_DEBUG__
		printf("MALLOC/FREE DEBUG malloc megarom\n");
		#endif
		if (megarom == NULL) {
			fprintf(stderr, "Out of memory loading %s\n", file);
			o2em_clean_quit(EXIT_FAILURE);
		}
		if (fread(megarom, l, 1, fn) != 1) {
			fprintf(stderr,"Error loading %s\n",file);
			o2em_clean_quit(EXIT_FAILURE);
		}
		/* mirror shorter files into full megabyte */
/*
if (l < 65536) memcpy(megarom+32768,megarom,32768);
if (l < 131072) memcpy(megarom+65536,megarom,65536);
if (l < 262144) memcpy(megarom+131072,megarom,131072);
if (l < 524288) memcpy(megarom+262144,megarom,262144);
if (l < 1048576) memcpy(megarom+524288,megarom,524288);
memcpy(&rom_table[0][1024], megarom + 4096*255 + 1024, 3072);
*/
		printf("MegaCart %ldK", l / 1024);
		nb = 1;
	} else if (((l % 3072) == 0)) {
		app_data->three_k = 1;
		nb = l / 3072;

		for (i = nb - 1; i >= 0; i--) {
			if (fread(&rom_table[i][1024], 3072, 1, fn) != 1) {
				fprintf(stderr, "Error loading %s\n", file);
				fclose(fn);
				o2em_clean_quit(EXIT_FAILURE);
			}
		}
		printf("%dK\n", nb * 3);

	} else {
		nb = l / 2048;
		if ((nb == 2) && (app_data->exrom)) {
			if (fread(&extROM[0], 1024, 1, fn) != 1) {
				fprintf(stderr, "Error loading %s\n", file);
				fclose(fn);
				return O2EM_FAILURE;
			}
			if (fread(&rom_table[0][1024], 3072, 1, fn) != 1) {
				fprintf(stderr, "Error loading %s\n", file);
				fclose(fn);
				return O2EM_FAILURE;
			}
			printf("3K EXROM\n");
		} else {
			for (i = nb - 1; i >= 0; i--) {
				nbread = fread(&rom_table[i][1024], 2048, 1, fn);
				if (nbread != 1) {
					fprintf(stderr, "Error reading %s %s\n", file, strerror(errno));
					fclose(fn);
					return O2EM_FAILURE;
				}
				memcpy(&rom_table[i][3072], &rom_table[i][2048], 1024);	/* simulate missing A10 */
			}
			printf("%dK\n", nb * 2);
		}
	}
	fclose(fn);
	/*
	   rom = rom_table[0]; */
	if (nb == 1)
		app_data->bank = 1;
	else if (nb == 2)
		app_data->bank = app_data->exrom ? 1 : 2;
	else if (nb == 4)
		app_data->bank = 3;
	else
		app_data->bank = 4;

	if ((rom_table[nb - 1][1024 + 12] == 'O') &&
	    (rom_table[nb - 1][1024 + 13] == 'P') && (rom_table[nb - 1][1024 + 14] == 'N') && (rom_table[nb - 1][1024 + 15] == 'B')) {
		app_data->openb = 1;
		printf("  openb ROM\n");
	}
	printf("  %d bank(s)\n", app_data->bank);
	/*
	   for (i = 0; i < 8; i++) {
	   for (nbread = 0; nbread < 1024; nbread++)
	   printf("%02x", rom_table[i][nbread]);
	   printf("\n");
	   } */
	return O2EM_SUCCESS;
}
/*===========================================================================*/
/*===========================================================================*/
int identify_rom(char *filename)
{
	FILE *gamelist;
	uint32_t crc;
	char buff[1024];
	uint32_t crcg;
	int romfound = 0;
	int i;

	if (filename == NULL) {
		fprintf(stderr, "%s filename is NULL\n", __func__);
		return O2EM_FAILURE;
	}
	crc = crc32_file(filename);
	printf("%s CRC %x\n", filename, crc);
	gamelist = fopen("gamelist.dat", "r");
	if (gamelist == NULL) {
		printf("%s cant open gamelist.dat\n", __func__);
		return O2EM_FAILURE;
	}
	while (fgets(buff, sizeof(buff), gamelist) != NULL) {
		for (i = 0; i < sizeof(buff) - 1 && buff[i] != '\r' && buff[i] != '\n' && buff[i] != '\0'; i++) {
			/* nothing*/
		}
		buff[i] = '\0';
		if (romfound == 1) {
			if(strncasecmp(buff, "NAME=", 5) == 0) {
				printf("Name: %s\n", buff + 5);
			}
			if(strcasecmp(buff, "BIOS_ODYSSEY2") == 0) {
				printf("Work on: BIOS_ODYSSEY2\n");
				app_data.need_bios += BIOS_ODYSSEY2;
			}
			if(strcasecmp(buff, "BIOS_G7400") == 0) {
				printf("Work on: BIOS_G7400\n");
				app_data.need_bios += BIOS_G7400;
			}
			if(strcasecmp(buff, "EXROM") == 0) {
				printf("is EXROM\n");
				app_data.exrom = 1;
			}
		}
		if(strncasecmp(buff, "CRC=", 4) == 0) {
			romfound = 0;
			crcg = strtoul(buff + 4, NULL, 16);
			/*printf("%s %x\n", buff + 4, crcg);*/
			if (crc == crcg) {
				romfound = 1;
			}
		}
	}
	fclose(gamelist);
	return O2EM_SUCCESS;
}

/*===========================================================================*/
/*===========================================================================*/
int identify_all_roms(char *pathx)
{
	DIR *dir_p;
	struct dirent *dir_entry_p;
	char path_tmp[1024];
	/*printf("%s!!!!!!!!!!!!!!!!!\n", __func__);*/

	if (pathx == NULL) {
		printf("bad usage of %s\n", __func__);
		return O2EM_FAILURE;
	}
	dir_p = opendir(pathx);
	if (dir_p == NULL) {
		fprintf(stderr, "dir '%s' not found !\n", pathx);
		return O2EM_FAILURE;
	}
	printf("Check directory %s\n", pathx);
	while (0 != (dir_entry_p = readdir(dir_p))) {
		/*TODO hard coded roms*/
		sprintf(path_tmp, "./roms/%s", dir_entry_p->d_name);
		printf("\ncheck %s\n", path_tmp);
		identify_rom(path_tmp);
	}
	closedir(dir_p);
	return O2EM_SUCCESS;
}
