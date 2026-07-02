/*
 *   O2EM2 Free Odyssey2 / Videopac+ Emulator
 *
 *   Created by Daniel Boris <dboris@comcast.net>  (c) 1997,1998
 *   Developed by Andre de la Rocha   <adlroc@users.sourceforge.net>
 *             Arlindo M. de Oliveira <dgtec@users.sourceforge.net>
 *
 *   Under development by LABBE Corentin http://o2em2.sourceforge.net
 *
 */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#include "crc32.h"
#include "audio.h"
#include "vmachine.h"
#include "o2em2.h"
#include "vdc.h"
#include "vpp.h"
#include "cpu.h"
#include "debug.h"
#include "keyboard.h"
#include "voice.h"
#ifndef __O2EM_SDL__
	#include "allegro.h"
	#ifdef ALLEGRO_WINDOWS
		#include "winalleg.h"
	#endif
#else
#include "o2em_sdl.h"
#endif
#include "score.h"
#include "roms.h"
#include "bios.h"

static char bios[MAXC], scshot[MAXC],
       full_path_to_rom[MAXC],
       scorefile[MAXC];
int helpflag;
/* We must differenciate o2em2 debug and game debug*/
int o2em_app_debug = 0;
int o2em_game_debug = 0;
static void helpus();
int parse_option(char *attr, char *val);
void read_default_config();





/*============================================================================*/
/*============================================================================*/
int main(int argc, char *argv[]){
	int i, j;
	/*int cnt, cntt, cnttt, way;*/
	static char file[MAXC], attr[MAXC], val[MAXC], *p, *binver;
	int stateError;
	int ret;
#if defined(ALLEGRO_WINDOWS)
	binver = "Windows binary";
#elif defined(ALLEGRO_DOS)
	binver = "DOS binary";
#elif defined(ALLEGRO_LINUX)
	binver = "Linux binary";
#elif defined(ALLEGRO_BEOS)
	binver = "BEOS binary";
#elif defined(ALLEGRO_QNX)
	binver = "QNX binary";
#elif defined(ALLEGRO_UNIX)
	binver = "UNIX binary";
#elif defined(ALLEGRO_MPW)
	binver = "MacOS binary";
#else
	#ifdef __O2EM_SDL__
	binver = "SDL binary";
	#else
	binver = "Unknown binary";
	#endif
#endif
	printf("%s %s\n","\nO2EM2 v" O2EM_VERSION " " RELEASE_DATE "  - ", binver);
	printf("Free Odyssey2 / Videopac+ Emulator - http://o2em2.sourceforge.net\n");
	printf("Created by Daniel Boris (c)1996/1998\n");
	printf("Developed by:\n");
	printf("     Andre de la Rocha since version 0.80\n");
	printf("     Arlindo M. de Oliveira since version 1.16\n");
	printf("     LABBE Corentin since version 1.5\n");
	printf("\n");
	memset(&app_data, 0, sizeof(app_data));
	app_data.debug = 0;
	app_data.stick[0] = app_data.stick[1] = 1;
	app_data.sticknumber[0] = app_data.sticknumber[1] = 0;
	set_defjoykeys(0, 0);
	set_defjoykeys(1, 1);
	set_defsystemkeys();
	app_data.bank = 0;
	app_data.limit = 1;
	app_data.sound_en = 1;
	app_data.speed = 100;
	app_data.wsize = 2;
	#ifdef ALLEGRO_DOS
	app_data.fullscreen = 1;
	#else
	app_data.fullscreen = 0;
	#endif
	app_data.scanlines = 0;
	app_data.voice = 0;
	app_data.window_title = "O2EM v" O2EM_VERSION;
	app_data.svolume = 100;
	app_data.vvolume = 100;
	app_data.filter = 0;
	app_data.exrom = 0;
	app_data.three_k = 0;
	app_data.crc = 0;
	app_data.scshot = scshot;
	app_data.euro = 0;
	app_data.openb = 0;
	app_data.vpp = 0;
	app_data.bios = 0;
	app_data.force_bios = 0;
	app_data.scoretype = 0;
	app_data.scoreaddress = 0;
	app_data.default_highscore = 0;
	app_data.breakpoint = 65535;
	app_data.megaxrom = 0;
	app_data.handleevbl = 0;
	app_data.handleevbll = 0;
	app_data.allowvdcalways = 0;
	app_data.c7420 = 0;
	app_data.disableJoyDefault = 0;
	strcpy(file, "");
	strcpy(scshot, "o2em_screenshot@.bmp");
	memset(app_data.statefile, 0, sizeof(app_data.statefile));
	strcpy(scorefile, "highscore.txt");
	memset(app_data.romdir, 0, sizeof(app_data.romdir));
	strcpy(app_data.romdir, "./roms/");
	memset(app_data.biosdir, 0, sizeof(app_data.biosdir));
	strcpy(app_data.biosdir, "./bios/");
	memset(app_data.bios_filename, 0, sizeof(app_data.bios_filename));
	strcpy(app_data.bios_filename, "o2rom.bin");
	memset(app_data.voice_path, 0, sizeof(app_data.voice_path));
	strcpy(app_data.voice_path, "./voice/");
	megarom = NULL;
	bmp = NULL;
	bmpcache = NULL;
	col = NULL;
	colplus = NULL;
	vppbmp = NULL;
	#ifdef __O2EM_SDL__
	SCREEN_W = 0;
	SCREEN_H = 0;
	DISPLAY_DEPTH = 8;
	screen = NULL;
	font = NULL;
	#endif
	for (i = 0; i < 9; i++)
		for (j = 0; j < 128; j++)
			voices[i][j] = NULL;

	read_default_config();
	if (argc <= 1) {
		helpus();
		o2em_clean_quit(EXIT_FAILURE);
	}
	/*param use*/
	for (i = 1; i < argc; i++) {
		/* if no - is here, its the rom wanted */
		if (argv[i][0] != '-') 	{
			strncat(file, argv[i], MAXC - 1);
			file[MAXC - 1] = '\0';
		} else {
			memset(attr, 0, MAXC);
			memset(val, 0, MAXC);
			p = strtok(argv[i] , "=");
			if (p != NULL ) {
				strncpy(attr, p + 1, MAXC - 1);/*TODO bad with multibyte locale*/
				attr[MAXC - 1] = '\0';
			}
			p = strtok(NULL, "=");
			if (p != NULL) {
				strncpy(val, p, MAXC - 1);
				val[MAXC - 1] = '\0';
			}
			strlwr(attr);
			if (strcasecmp(attr, "identify") == 0) {
				identify_rom(argv[i + 1]);
				o2em_clean_quit(EXIT_SUCCESS);
			}
			if (strcasecmp(attr, "list") == 0) {
				if (val[0] != '\0')
					identify_all_roms(val);
				else
					identify_all_roms("./roms");
				o2em_clean_quit(EXIT_SUCCESS);
			}
			if (parse_option(attr, val) == O2EM_FAILURE)
				o2em_clean_quit(EXIT_FAILURE);
		}
	}
	if (helpflag)
		helpus();
	if (strlen(file) == 0) {
		fprintf(stderr, "Error: file name missing\n");
		o2em_clean_quit(EXIT_FAILURE);
	}


	/* First load cart then load bios according to cart needs 
	 * If a cart can support multiple BIOS we try first more advanced BIOS (ie prefer VPP other classic VDC)
	 * */
	ret = search_for_rom(app_data.romdir, file, full_path_to_rom);
	if (ret == O2EM_FAILURE) {
		printf("No ROM found\n");
		o2em_clean_quit(EXIT_FAILURE);
	}
	ret = load_cart(full_path_to_rom, &app_data);
	if (ret == O2EM_FAILURE) {
		printf("Error of load_cart()\n");
		o2em_clean_quit(EXIT_FAILURE);
	}
	/* Now we have loaded the wanted ROM */
	if (app_data.force_bios > 0)
		app_data.need_bios = app_data.force_bios;

	if ((app_data.need_bios & BIOS_G7400) > 0)
		ret = search_for_bios(app_data.biosdir, bios, BIOS_G7400);
	if ((app_data.need_bios & BIOS_JOPAC) > 0 && bios[0] == '\0')
		ret = search_for_bios(app_data.biosdir, bios, BIOS_JOPAC);
	if ((app_data.need_bios & BIOS_C52) > 0 && bios[0] == '\0')
		ret = search_for_bios(app_data.biosdir, bios, BIOS_C52);
	if ((app_data.need_bios & BIOS_ODYSSEY2) > 0 && bios[0] == '\0')
		ret = search_for_bios(app_data.biosdir, bios, BIOS_ODYSSEY2);
	if (bios[0] == '\0') {
		ret = search_for_bios(app_data.biosdir, bios, 0);
		if (ret == O2EM_FAILURE) {
			printf("No BIOS found\n");
			o2em_clean_quit(EXIT_FAILURE);
		}
	}
	#ifdef __O2EM_DEBUG__
	printf("DEBUG -> BIOS -> %s\n", bios);
	#endif
	ret = load_bios(bios, rom_table, &app_data);
	if (ret == O2EM_FAILURE) {
		printf("Error of load_bios()\n");
		o2em_clean_quit(EXIT_FAILURE);
	}
	/* now check that loaded bios and roms can work togeher */
	if ((app_data.bios & app_data.need_bios) == 0) {
		printf("Warning Bios found is imcompatible with that rom\n");
		/*o2em_clean_quit(EXIT_FAILURE);*/
	}

	/*printf("uint=%lu ulong=%lu ushort=%lu\n", sizeof(unsigned int), sizeof(unsigned long), sizeof(unsigned short));*/
	printf("Starting emulation ...\n");
	allegro_init();
	ret = o2em_init_keyboard();
	if (ret == O2EM_FAILURE)
		o2em_clean_quit(EXIT_FAILURE);
	ret = install_timer();
	if (ret != 0) {
		printf("  Error install_timer() %s %s\n", strerror(errno), allegro_error);
		o2em_clean_quit(EXIT_FAILURE);
	}
	ret = o2em_init_audio();
	#ifdef __O2EM_DEBUG__
	printf("DEBUG: after init_audio() %d\n", ret);
	#endif
#ifndef __O2EM_SDL__
	printf("Using Allegro %s\n", allegro_id);
#endif

	if (app_data.voice) {
		ret = load_voice_samples(app_data.voice_path);
		if (ret == O2EM_FAILURE) {
			printf("Error of load_voice_samples()\n");
			/*return EXIT_FAILURE;*/
		}
	}

	ret = init_display();
	if (ret == O2EM_FAILURE) {
		printf("Error of init_display()\n");
		return EXIT_FAILURE;
	}

	init_cpu();
	init_system();
	set_score(app_data.scoretype, app_data.scoreaddress, app_data.default_highscore);
	if (app_data.statefile[0] != '\0') {
		stateError = loadstate(app_data.statefile);
		if (stateError == O2EM_SUCCESS)
			printf("Savefile loaded.");
		else {
			if (stateError >= 199) {
				if (stateError == 199) fprintf(stderr, "Wrong ROM-File for Savefile.");
				else if (stateError == 200 + ROM_O2) fprintf(stderr, "Wrong BIOS for Savefile: O2ROM needed.");
				else if (stateError == 200 + ROM_G7400) fprintf(stderr, "Wrong BIOS for Savefile: G7400 ROM needed.");
				else if (stateError == 200 + ROM_C52) fprintf(stderr, "Wrong BIOS for Savefile: C52 ROM needed.");
				else if (stateError == 200 + ROM_JOPAC) fprintf(stderr, "Wrong BIOS for Savefile: JOPAC ROM needed.");
				else fprintf(stderr, "Wrong BIOS for Savefile: UNKNOWN ROM needed.");
				return EXIT_FAILURE;
			}
		}
	}
	/*
	   if (app_data.debug) key_debug=1;
#ifndef _DEBUG
#ifdef ALLEGRO_WINDOWS
FreeConsole();
#endif
#endif
*/
	run();
	if (app_data.scoretype != 0)
		save_highscore(get_score(app_data.scoretype, app_data.scoreaddress), scorefile);
	#ifdef __O2EM_DEBUG__
	printf("DEBUG SCORE type=%d addr=%d %d\n", app_data.scoretype,
		app_data.scoreaddress,
		get_score(app_data.scoretype, app_data.scoreaddress));
	#endif
	o2em_clean_quit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
END_OF_MAIN()

/*============================================================================*/
/*============================================================================*/
int parse_option(char *attr, char *val)
{
	int t;
	char *p, *s;
	int i, k, code, nk, codes[12];
	#ifdef __O2EM_DEBUG__
	printf("%s %s %s\n", __func__, attr, val);
	#endif

	if (strcasecmp(attr, "nolimit") == 0) {
		app_data.limit = !(val[0] != '0');
		return O2EM_SUCCESS;
	}
	if (strcasecmp(attr, "showfps") == 0) {
		app_data.show_fps = (val[0] != '0');
		#ifdef __O2EM_DEBUG__
		printf("On passe show_fps a %d\n", app_data.show_fps);
		#endif
		return O2EM_SUCCESS;
	}
	if (strcasecmp(attr, "nosound") == 0) {
		app_data.sound_en = !(val[0] != '0');
		return O2EM_SUCCESS;
	}
	if (strcasecmp(attr, "novoice") == 0) {
		app_data.voice = !(val[0] != '0');
		return O2EM_SUCCESS;
	}
	/* TODO parameter handleevbll */
	/* TODO parameter handleevbl */
	if (strcasecmp(attr, "filter") == 0) {
		app_data.filter = (val[0] != '0');
	} else if (strcasecmp(attr, "debug") == 0) {
		app_data.debug = (val[0]!='0');
	} else if ((!strcmp(attr,"s1")) || (!strcmp(attr,"s2"))) {
		int sn;
		sn = (!strcmp(attr,"s1"))? 0 : 1;
		if (strlen(val)<2){
			t = -1;
			sscanf(val,"%d",&t);
			if ((t>=0) && (t<=3)) {
				if ((t==1)||(t==2)){
					app_data.stick[sn] = 1;
					set_defjoykeys(sn, t-1);
				} else {
					app_data.stick[sn] = (t==0) ? 0 : 2;
					if (t==3)
					{
						app_data.sticknumber[sn] = app_data.sticknumber[0]+app_data.sticknumber[1]+1;
					}
					set_joykeys(sn, 0, 0, 0, 0, 0);
				}
			} else {
				fprintf(stderr,"Invalid value for option %s\n", attr);
				return O2EM_FAILURE;
			}
		} else {
			strupr(val);
			nk = 0;
			p = strtok(val, ",");
			while (p) {
				i = code = 0;
				k = keybtab[i].keybcode;
				s = keybtab[i].keybname;
				while (k && (code == 0)) {
					if (strcmp(s, p) == 0)
						code = k;
					i++;
					k = keybtab[i].keybcode;
					s = keybtab[i].keybname;
				}
				if (code == 0) {
					fprintf(stderr, "Invalid value for option %s : key %s unknown\n", attr, p);
					return O2EM_FAILURE;
				}
				codes[nk] = code;
				p = strtok(NULL, ",");
				nk++;
				if (nk > 5) {
					fprintf(stderr, "Invalid value for option %s : invalid number of keys %d\n", attr, nk);
					return O2EM_FAILURE;
				}
			}
			if (nk != 5) {
				fprintf(stderr,"Invalid value for option %s : invalid number of keys\n",attr);
				return O2EM_FAILURE;
			}
			app_data.stick[sn] = 1;
			set_joykeys(sn, codes[0], codes[1], codes[2], codes[3], codes[4]);
		}
	}
	if (strcmp(attr, "s0") == 0) {
		strupr(val);
		nk = 0;
		p = strtok(val, ",");
		while (p) {
			i = code = 0;
			k = keybtab[i].keybcode;
			s = keybtab[i].keybname;
			while (k && (code == 0)) {
				if (strcmp(s,p)==0) code = k;
				i++;
				k = keybtab[i].keybcode;
				s = keybtab[i].keybname;
			}
			if (!code) {
				fprintf(stderr,"Invalid value for option %s : key %s unknown\n",attr,p);
				return O2EM_FAILURE;
			}
			codes[nk] = code;
			p = strtok(NULL, ",");
			nk++;
			if (nk > 8) {
				fprintf(stderr, "Invalid value for option %s : invalid number of keys %d\n", attr, nk);
				return O2EM_FAILURE;
			}
		}
		if (nk != 8) {
			fprintf(stderr, "Invalid value for option %s : invalid number of keys %d\n", attr, nk);
			return O2EM_FAILURE;
		}
		set_systemkeys(codes[0], codes[1], codes[2], codes[3], codes[4], codes[5], codes[6], codes[7],codes[8],codes[9],codes[10],codes[11]);

	}
	if (strcasecmp(attr, "speed") == 0) {
		t = -1;
		sscanf(val,"%d",&t);
		if ((t > 0) && (t <= 10000))
			app_data.speed = t;
		else {
			fprintf(stderr, "Invalid value for option %s\n", attr);
			return O2EM_FAILURE;
		}
	} else if (!strcmp(attr,"svolume")) {
		t = -1;
		sscanf(val,"%d",&t);
		if ((t >= 0) && (t <= 100))
			app_data.svolume = t;
		else {
			fprintf(stderr, "Invalid value for option %s\n", attr);
			return O2EM_FAILURE;
		}
		if (t == 0)
			app_data.sound_en = 0;
	} else if (!strcmp(attr,"vvolume")) {
		t = -1;
		sscanf(val,"%d",&t);
		if ((t>=0) && (t<=100))
			app_data.vvolume = t;
		else {
			fprintf(stderr, "Invalid value for option %s\n", attr);
			return O2EM_FAILURE;
		}
		if (t==0) app_data.voice=0;
	} else if (!strcmp(attr,"wsize")) {
		t = -1;
		sscanf(val,"%d",&t);
		if ((t>0) && (t<5)) {
			app_data.wsize = t;
			app_data.fullscreen = 0;
		} else {
			fprintf(stderr, "Invalid value for option %s\n", attr);
			return O2EM_FAILURE;
		}
	} else if (!strcmp(attr,"fullscreen")) {
		app_data.fullscreen = (val[0]!='0');
	} else if (!strcmp(attr, "scanlines")) {
		app_data.scanlines = (val[0]!='0');
	} else if (!strcmp(attr, "scshot")) {
		strcpy(scshot, val);
	} else if (strcasecmp(attr, "savefile") == 0) {
		strcpy(app_data.statefile, val);
	}
	if (strcasecmp(attr, "biosdir") == 0) {
		strcpy(app_data.biosdir, val);
	}
	if (strcasecmp(attr, "bios") == 0) {
		strcpy(app_data.bios_filename, val);
	}
	if (!strcmp(attr, "romdir")) {
		strcpy(app_data.romdir, val);
	} else if (!strcmp(attr, "help")) {
		helpflag = 1;
	} else if (!strcmp(attr, "h")) {
		helpflag = 1;
	} else if (!strcmp(attr, "euro")) {
		app_data.euro = (val[0]!='0');
	} else if (!strcmp(attr, "exrom")) {
		app_data.exrom = (val[0]!='0');
	} else if (!strcmp(attr, "3k")) {
		app_data.three_k = (val[0]!='0');
	}
	if (strcasecmp(attr, "g7400") == 0) {
		app_data.force_bios = BIOS_G7400;
		printf("Setting BIOS to G7400\n");
	} else if (!strcmp(attr, "c52")) {
		app_data.force_bios = BIOS_C52;
		printf("Setting BIOS to C52\n");
	} else if (!strcmp(attr,"jopac")) {
		app_data.force_bios = BIOS_JOPAC;
		printf("Setting BIOS to JOPAC\n");
	} else if (!strcmp(attr, "o2rom")) {
		app_data.force_bios = BIOS_ODYSSEY2;
		printf("Setting BIOS to ODYSSEY2\n");
	} else if (!strcmp(attr, "scorefile")) {
		strcpy(scorefile, val);
	} else if (!strcmp(attr, "scoreadr")) {
		t = -1;
		sscanf(val, "%d", &t);
		if ((t >= 0) && (t <= 255))
			app_data.scoreaddress = t;
		else {
			fprintf(stderr,"Invalid value for option %s\n",attr);
			return O2EM_FAILURE;
		}
	} else if (!strcmp(attr, "scoretype")) {
		t = -1;
		sscanf(val, "%d", &t);/* TODO atoi, atol, strtoul, and co*/
		if ((t >= 0) && (t <= 9999))
			app_data.scoretype = t;
		else {
			fprintf(stderr, "Invalid value for option %s\n", attr);
			return O2EM_FAILURE;
		}
	} else if (!strcmp(attr, "score")) {
		t = -1;
		sscanf(val, "%d", &t);
		if ((t >= 0) && (t <= 999999))
			app_data.default_highscore = t;
		else {
			fprintf(stderr, "Invalid value for option %s\n", attr);
			return O2EM_FAILURE;
		}
	} else {
		if (!helpflag){
			fprintf(stderr, "Invalid option : %s\n", attr);
			return O2EM_FAILURE;
		}
	}
	return O2EM_SUCCESS;
}

/*============================================================================*/
/*============================================================================*/
void read_default_config()
{
	FILE *f;
	static char attr[MAXC], val[MAXC], s[MAXC];
	char *p, *fn;
	int i, l;
	fn = "o2em_def.cfg";
	f = fopen(fn, "r");
	if (f == NULL) {
		fn = "O2EM_DEF.CFG";
		f = fopen(fn, "r");
	}
	if (f == NULL) {
		printf("Cant find o2em_def.cfg, falling back to default values\n");
		return;
	}

	l = 0;
	while (fgets(s, MAXC - 1, f)) {
		l++;
		p = s;
		while (*p && (isspace(*p)))
			p++;
		if (*p && (*p != '#')) {
			i = 0;
			while (*p && (!isspace(*p)) && (*p != '=')) attr[i++] = *p++;
			attr[i] = 0;
			while (*p && (isspace(*p))) p++;
			i = 0;
			if (*p == '=') {
				p++;
				while (*p && (isspace(*p))) p++;
				if (*p == '"') {
					p++;
					while (*p && (*p != '"') && (*p != '\n') && (*p != '\r')) val[i++] = *p++;
				} else {
					while (*p && (!isspace(*p))) val[i++] = *p++;
				}
			}
			val[i] = 0;
			if (strlen(attr) > 0) {
				strlwr(attr);
				if (!parse_option(attr, val))
					printf("Error in the %s file at line number %d !\n\n", fn, l);
			}
		}
	}
	fclose(f);
}

/*============================================================================*/
/*============================================================================*/
void helpus()
{
	/*
	allegro_init();
	install_timer();
	init_display();
	init_system();
	#ifdef ALLEGRO_WINDOWS
	FreeConsole();
	#endif*/
	help();
	o2em_clean_quit(EXIT_SUCCESS);
}

/*============================================================================*/
/*============================================================================*/
void o2em_clean_quit(int exitcode)
{
	int i, j;
	/*
	remove_keyboard();
	remove_timer();
	remove_joystick();*/
	/* http://www.allegro.cc/manual/4/api/sound-init-routines/remove_sound */
	/*remove_sound();*/
	for (i = 0; i < 9; i++)
		for (j = 0; j < 128; j++)
			if (voices[i][j] != NULL) {
				destroy_sample(voices[i][j]);
				voices[i][j] = NULL;
			}
	if (bmpcache != NULL) {
		#ifdef __O2EM_MEM_DEBUG__
		printf("MALLOC/FREE DEBUG free bmpcache\n");
		#endif
		#ifndef __O2EM_SDL__
		destroy_bitmap(bmpcache);
		#else
		SDL_FreeSurface(bmpcache);
		#endif
	}
	if (bmp != NULL) {
		#ifdef __O2EM_MEM_DEBUG__
		printf("MALLOC/FREE DEBUG free bmp\n");
		#endif
		#ifndef __O2EM_SDL__
		destroy_bitmap(bmp);
		#else
		SDL_FreeSurface(bmp);
		#endif
	}
	if (vppbmp != NULL) {
		#ifdef __O2EM_MEM_DEBUG__
		printf("MALLOC/FREE DEBUG free vppbmp\n");
		#endif
		#ifndef __O2EM_SDL__
		destroy_bitmap(vppbmp);
		#else
		SDL_FreeSurface(vppbmp);
		#endif
	}

	if (megarom != NULL) {
		#ifdef __O2EM_MEM_DEBUG__
		printf("MALLOC/FREE DEBUG free megarom\n");
		#endif
		free(megarom);
	}
	if (col != NULL) {
		#ifdef __O2EM_MEM_DEBUG__
		printf("MALLOC/FREE DEBUG free col\n");
		#endif
		free(col);
	}

	if (colplus != NULL) {
		#ifdef __O2EM_MEM_DEBUG__
		printf("MALLOC/FREE DEBUG free colplus\n");
		#endif
		free(colplus);
	}
	#ifdef __O2EM_SDL__
	SDL_Quit();
	#else
	allegro_exit();
	#endif
	exit(exitcode);
}

