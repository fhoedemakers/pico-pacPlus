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
#include <dirent.h>
#include "bios.h"
#include "crc32.h"
#include "o2em2.h"

#define DEFAULT_BIOS_DIR "bios/"

/*===========================================================================*/
/*===========================================================================*/
/* search for a BIOS, if bios_type = 0 any BIOS is taked */
int search_for_bios(char *pathx, char *bios_found, int bios_type)
{
	DIR *dir_p;
	struct dirent *dir_entry_p;
	uint32_t crc;

	if (pathx == NULL || bios_found == NULL) {
		printf("bad usage of %s\n", __func__);
		return O2EM_FAILURE;
	}
	dir_p = opendir(pathx);
	if (dir_p == NULL) {
		fprintf(stderr, "dir '%s' not found !\n", pathx);
		return O2EM_FAILURE;
	}
	#ifdef __O2EM_VERBOSE__
	printf("Check directory %s for BIOS type %d\n", pathx, bios_type);
	#endif
	while (NULL != (dir_entry_p = readdir(dir_p))) {
		if (strcmp(dir_entry_p->d_name, ".") != 0 && strcmp(dir_entry_p->d_name, "..") != 0) {
			memset(bios_found, 0, MAXC);
			snprintf(bios_found, MAXC, "%s/%s", pathx, dir_entry_p->d_name);
			#ifdef __O2EM_VERBOSE__
			printf("  Check %s\n", dir_entry_p->d_name);
			#endif
			crc = crc32_file(bios_found);
			switch (bios_type) {
				case 0:
					if (crc == CRC_ODYSSEY2 || crc == CRC_G7400 || crc == CRC_C52 || crc == CRC_JOPAC) {
						closedir(dir_p);
						return O2EM_SUCCESS;
					}
					break;
				case BIOS_ODYSSEY2:
					if (crc == CRC_ODYSSEY2) {
						closedir(dir_p);
						return O2EM_SUCCESS;
					}
					break;
				case BIOS_G7400:
					if (crc == CRC_G7400) {
						closedir(dir_p);
						return O2EM_SUCCESS;
					}
					break;
				case BIOS_C52:
					if (crc == CRC_C52) {
						closedir(dir_p);
						return O2EM_SUCCESS;
					}
					break;
				case BIOS_JOPAC:
					if (crc == CRC_JOPAC) {
						closedir(dir_p);
						return O2EM_SUCCESS;
					}
					break;
				default:
					printf("Unknown parameter\n");
					break;
			}
		}
	}
	closedir(dir_p);
	return O2EM_FAILURE;
}

/********************* Ready BIOS
 *
 * param biossux filename to the bios
 * param app_data resource data
 * */
/* TODO seems obsoletes*/
int identify_bios(char *biossux, struct resource *app_data)
{
	#ifdef __O2EM_DEBUG__
	printf("DEBUG %s bios=%s\n", __func__, biossux);
	#endif
	app_data->crc = crc32_file(biossux);
	if (app_data->crc == 0x8016A315) {
/*		strcpy(odyssey2, biossux);*/
/*		o2flag = 1;*/
		return BIOS_ODYSSEY2;
	}
	if (app_data->crc == 0xE20A9F41) {
/*		strcpy(g7400, biossux);*/
		/*g74flag = 1; */
		return BIOS_G7400;
	}
	if (app_data->crc == 0xA318E8D6) {
/*		strcpy(c52, biossux);*/
/*		c52flag = 1;*/
		return BIOS_C52;
	}
	if (app_data->crc == 0x11647CA5) {
/*		strcpy(jopac, biossux);*/
/*		jopflag = 1;*/
		return BIOS_JOPAC;
	}
	return O2EM_FAILURE;/* all BIOS_ is > 0*/
}

/*===========================================================================*/
/*===========================================================================*/
int load_bios(const char *biosname, unsigned char **romtable, struct resource *app_data)
{
	FILE *fn = NULL;
	uint32_t crc;
	int i;
	size_t nbread;
/*
        if ((biosname[strlen(biosname)-1]=='/') ||
           (biosname[strlen(biosname)-1]=='\\') ||
           (biosname[strlen(biosname)-1]==':')) {
                strcpy(s,biosname);
                strcat(s,odyssey2);
                fn = fopen(s,"rb");

                if (!fn) {
                        strcpy(s,biosname);
                        strcat(s,odyssey2);
                        fn = fopen(s,"rb");
                        }
              } else {

        strcpy(s,biosname);
                fn = fopen(biosname,"rb");
        }
*/
	fn = fopen(biosname, "rb");
	if (fn == NULL) {
		fprintf(stderr, "Error loading bios ROM (%s): %s\n", biosname, strerror(errno));
		return O2EM_FAILURE;
	}
	nbread = fread(rom_table[0], 1024, 1, fn);
	if (nbread != 1) {
		/*fprintf(stderr, "Error reading bios ROM %s\n",odyssey2); */
		fprintf(stderr, "Error reading bios ROM (%s) %s\n", biosname, strerror(errno));
		fclose(fn);
		return O2EM_FAILURE;
	}
	fclose(fn);
	/*TODO why doing 8 copy ? */
	for (i = 1; i < 8; i++)
		memcpy(rom_table[i], rom_table[0], 1024);
	crc = crc32_buf(rom_table[0], 1024);
	if (crc == 0x8016A315) {
		printf("Odyssey2 bios ROM loaded\n");
		app_data->vpp = 0;
		app_data->bios = ROM_O2;
	} else if (crc == 0xE20A9F41) {
		printf("Videopac+ G7400 bios ROM loaded\n");
		app_data->vpp = 1;
		app_data->bios = ROM_G7400;
	} else if (crc == 0xA318E8D6) {
		/* TODO why these test ?
		   if (!((!o2flag)&&(c52flag))) */
		printf("C52 bios ROM loaded\n");
/*		else
			printf("Ok\n");*/
		app_data->vpp = 0;
		app_data->bios = ROM_C52;

	} else if (crc == 0x11647CA5) {
		/*if (g74flag) */
		printf("Jopac bios ROM loaded\n");
		/*else printf(" Ok\n"); */
		app_data->vpp = 1;
		app_data->bios = ROM_JOPAC;
	} else {
		printf("Bios ROM loaded (unknown version)\n");
		app_data->vpp = 0;
		app_data->bios = ROM_UNKNOWN;
	}
	return O2EM_SUCCESS;
}
