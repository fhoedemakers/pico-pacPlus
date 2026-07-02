/*
################################################################################
#                                                                              #
#   Copyright (C) 2010-2013 LABBE Corentin <clabbe.montjoie@gmail.com>
#
#    O2EM2-SDL is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    O2EM2-SDL is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with O2EM2-SDL.  If not, see <http://www.gnu.org/licenses/>.
#                                                                              #
################################################################################
*/

#ifdef __O2EM_SDL__

#include <errno.h>
#include <string.h>
#include "o2em_sdl.h"
#include "o2em2.h"
#include <SDL/SDL_gfxPrimitives.h>

/*
 * window_close_hook
 * set_window_close_button
 *
 * */

/*===========================================================================*/
/*===========================================================================*/
BITMAP *create_bitmap(int w, int h)
{
	SDL_Surface *temp;
	int rmask, gmask, bmask, ret;

	if (w <= 0 || h <= 0) {
		fprintf(stderr, "%s invalid parameters %d,%d\n", __func__, w, h);
		return NULL;
	}
	printf("%s create with %dx%d:%d\n", __func__, w, h, DISPLAY_DEPTH);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
#endif

	temp = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, DISPLAY_DEPTH,
		 rmask, gmask, bmask, 0);

	if (temp == NULL) {
		fprintf(stderr, "%s %s %s %s\n", __FILE__, __func__, strerror(errno), SDL_GetError());
		return NULL;
	}
	/*printf("BytesPerPixel=%d\n", val_temp->format->BytesPerPixel);*/
	/*
	for (i = 0 ; i < 256; i++) {
		printf("%d %d %d\n", colors[ret].r, colors[ret].g, colors[ret].b);
	}
	check_palette(colors);
	*/
	ret = SDL_SetPalette(temp, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);
	printf("%s TODO SDL_SetPalette=%d check return\n", __func__, ret);
	return temp;
}

/*===========================================================================*/
/*===========================================================================*/
void clear(BITMAP *bitmap)
{
	if (bitmap != NULL)
		SDL_FillRect(bitmap, 0, 0);
	/*TODO handle error*/
}

/*===========================================================================*/
/*===========================================================================*/
/* http://www.allegro.cc/manual/4/api/keyboard-routines/keyboard_needs_poll */
int keyboard_needs_poll()
{
	return TRUE;
}
/*===========================================================================*/
/*===========================================================================*/
int poll_keyboard()
{
	SDL_Event event;
	#ifdef __O2EM_KEYBOARD_DEBUG__
	int i;
	#endif

	SDL_PumpEvents();
	while (SDL_PollEvent(&event) > 0) {
		switch (event.type) {
			case SDL_QUIT:
				o2em_clean_quit(EXIT_SUCCESS);
			default:
				break;
		}
	}
	key = SDL_GetKeyState(NULL);
	if (key == NULL)
		return O2EM_FAILURE;
	#ifdef __O2EM_KEYBOARD_DEBUG__
	for (i = 0; i < SDLK_LAST; i++)
		if (key[i] > 0 && i != SDLK_NUMLOCK)
			printf("appuie sur %d %s\n", i, SDL_GetKeyName(i));
	#endif
	/* hack for handling french keyboard http://sdl.beuc.net/sdl.wiki/SDLKey*/
	if (key[SDLK_AMPERSAND] > 0) {
		key[SDLK_1] = 1;
	}
	if (key[SDLK_WORLD_73] > 0) {
		key[SDLK_2] = 1;
	}
	if (key[SDLK_QUOTEDBL] > 0) {
		key[SDLK_3] = 1;
	}
/*
	for (i = 0; i < SDLK_LAST; i++) {
		if (key[i] > 0 && i != 300)
			printf("%d %d\n", i, key[i]);
	}*/
	return O2EM_SUCCESS;
}

/*===========================================================================*/
/*===========================================================================*/
int install_joystick(int joytype)
{
    return 0;
}

/*===========================================================================*/
/*===========================================================================*/
int install_keyboard()
{
	return 0;
}
/*===========================================================================*/
/*===========================================================================*/
void remove_keyboard()
{
}

/*===========================================================================*/
/*===========================================================================*/
void set_color_depth(int depth)
{
	if (depth == 8 || depth == 16 || depth == 24 || depth ==32)
		DISPLAY_DEPTH = depth;
	else
		fprintf(stderr, "%s Unknown DISPLAY_DEPTH %d\n", __func__, depth);
}

/*===========================================================================*/
/*===========================================================================*/
void allegro_init()
{
	int flags = SDL_INIT_VIDEO;/* TODO SDL_INIT_JOYSTICK */

	memset(allegro_error, 0, sizeof(allegro_error));
	printf("%s SDL_init\n", __func__);
	if (SDL_Init(flags) != 0) {
		printf("Unable to initialize SDL: %s", SDL_GetError());
		o2em_clean_quit(EXIT_FAILURE);
	}
}

/*===========================================================================*/
/*===========================================================================*/
/* emulate allegro set_gfx_mode */
int set_gfx_mode(int card, int w, int h, int v_w, int v_h)
{
	int video_depth = DISPLAY_DEPTH;
	int flags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF;/* TODO perhaps add SDL_RESIZABLE*/

	SCREEN_H = h;
	SCREEN_W = w;
	/*printf("%s video_depth=%d %p\n", __func__, video_depth, (void *)screen);*/
	if (screen == NULL)  {
		SDL_ShowCursor(0);
		if (card == GFX_AUTODETECT_FULLSCREEN)
			flags |= SDL_FULLSCREEN;
			screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, video_depth, flags);
	} else {
		fprintf(stderr, "Screen already init\n");
		return O2EM_FAILURE;
	}
	if (screen == NULL) {
		fprintf(stderr, "Unable to create screen: %s", SDL_GetError());
		return O2EM_FAILURE;
	}
	/*printf("%s final=%p\n", __func__, screen);*/
	return O2EM_SUCCESS;
}
/*===========================================================================*/
/*===========================================================================*/
void rest(unsigned int time)
{
	SDL_Delay(time);
}

/*===========================================================================*/
/*===========================================================================*/
void stretch_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int source_width, int source_height,
	int dest_x, int dest_y, int dest_width, int dest_height)
{
	int ret;
	SDL_Rect srcrect, dstrect;
	if (source == NULL || dest == NULL) {
		fprintf(stderr, "%s ERROR arg is NULL\n", __func__);
		return ;
	}
	srcrect.x = source_x;
	srcrect.y = source_y;
	srcrect.w = source_width;
	srcrect.h = source_height;
	dstrect.x = dest_x;
	dstrect.y = dest_y;
	dstrect.w = dest_width;
	dstrect.h = dest_height;
	ret = SDL_BlitSurface(source, &srcrect, dest, &dstrect);
	if (ret != 0) {
		fprintf(stderr, "%s error %s\n", __func__, SDL_GetError());
	}
}

/*===========================================================================*/
/*===========================================================================*/
void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color)
{
	SDL_Rect dst;
	dst.x = x1;
	dst.y = y1;
	dst.w = x2 - x1;
	dst.h = y2 - y1;

	SDL_FillRect(bmp, &dst, color);
}


/*===========================================================================*/
/*===========================================================================*/
void line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color)
{
}


/*===========================================================================*/
/*===========================================================================*/
void get_palette(SDL_Color *p)
{
}

/*===========================================================================*/
/*===========================================================================*/
int check_palette(SDL_Color *p)
{
	int i;
	for (i = 0; i < 64; i++) {
		if (p[i].r == 0 && p[i].g == 0 &&  p[i].b == 0) {
			fprintf(stderr, "%s palette is empty\n", __func__);
			return 1;
		}
	}
	return 0;
}

/*===========================================================================*/
/*===========================================================================*/
void set_palette(SDL_Color *p)
{
	printf("%s\n", __func__);
/*	memcpy(global_palette, p, sizeof(global_palette));*/
	check_palette(p);
	SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, p, 0, 256);
}


/*===========================================================================*/
/*===========================================================================*/
void textout_centre_ex(BITMAP *bmp, void *f, const char *s, int x, int y, int color, int bg)
{
	if (bmp == NULL) {
		fprintf(stderr, "%s bmp is NULL\n", __func__);
		return;
	}
	printf("%s %s\n", __func__, s);
}


/*===========================================================================*/
/*===========================================================================*/
void acquire_screen()
{
	if (SDL_MUSTLOCK(screen) == SDL_TRUE) {
		if ( SDL_LockSurface(screen) < 0 ) {
			return;
		}
	}
}

/*===========================================================================*/
/*===========================================================================*/
void release_screen()
{
/*	int i;*/

	if (SDL_MUSTLOCK(screen) == SDL_TRUE) {
		SDL_UnlockSurface(screen);
	}

/*	for (i = 0; i < 300 * 20; i++)
		*((unsigned char *)screen->pixels + i) = rand()%254;*/
	SDL_Flip(screen);
/*	printf("%s %p\n",  __func__, screen);*/
}

/*===========================================================================*/
/*===========================================================================*/
void textprintf_ex(BITMAP *bmp, void *f, int x, int y, int color, int bg, const char *fmt, ...)
{
	if (bmp == NULL) {
		fprintf(stderr, "%s bmp is NULL\n", __func__);
		return;
	}
	/*printf("%s %s\n", __func__, s);*/
}


/*===========================================================================*/
/*===========================================================================*/
void set_window_title(char *name)
{
	if (name == NULL) {
		fprintf(stderr, "%s error name is NULL\n", __func__);
		return;
	}
	SDL_WM_SetCaption(name, 0);
}
/*===========================================================================*/
/*===========================================================================*/
unsigned char *get_raw_pixel_line(SDL_Surface *pSurface, int y) {
	if (pSurface == NULL) {
		fprintf(stderr, "%s Error surface is NULL\n", __func__);
		return NULL;
	}
	if (y < 0) {
		fprintf(stderr, "%s Error y < 0\n", __func__);
		return NULL;
	}
	return (unsigned char *)pSurface->pixels + ( pSurface->pitch * y );
}

/*===========================================================================*/
/*===========================================================================*/
unsigned char *get_raw_pixel(SDL_Surface *pSurface, int x, int y) {
	if (pSurface == NULL) {
		fprintf(stderr, "%s Error surface is NULL\n", __func__);
		return NULL;
	}
	if (y < 0) {
		fprintf(stderr, "%s Error y < 0\n", __func__);
		return NULL;
	}
	return (unsigned char *)pSurface->pixels + ( pSurface->pitch * y ) + x;
}


/*===========================================================================*/
/*===========================================================================*/
int install_timer()
{
	/*TODO check remove_timer*/
	return 0;
}

/*===========================================================================*/
/*===========================================================================*/
int set_display_switch_mode(int mode)
{
	return 0;
}
/*===========================================================================*/
/*===========================================================================*/
char *strlwr(char *str)
{
	int i;
	if (str == NULL) {
		fprintf(stderr, "%s error str is NULL\n", __func__);
		return NULL;
	}
	for (i = 0; i < strlen(str); i++)
		str[i] = tolower(str[i]);
	return str;
}
/*===========================================================================*/
/*===========================================================================*/
char *strupr(char *str)
{
	int i;
	if (str == NULL) {
		fprintf(stderr, "%s error str is NULL\n", __func__);
		return NULL;
	}
	for (i = 0; i < strlen(str); i++)
		str[i] = toupper(str[i]);
	return str;
}

/*===========================================================================*/
/*===========================================================================*/

/*===========================================================================*/
/*===========================================================================*/

/*===========================================================================*/
/*===========================================================================*/













/*===========================================================================*/
/*===========================================================================*/
/* return -1 if failure*/
int install_sound(int digi, int midi, const char *cfg_path)
{
	int ret;
	ret = SDL_InitSubSystem(SDL_INIT_AUDIO);
	if (ret != 0) {
		printf("Unable to initialize SDL_AUDIO: %s", SDL_GetError());
		return -1;
	}
	return 0;
}

/*===========================================================================*/
/*===========================================================================*/
void remove_sound()
{
	SDL_CloseAudio();
}

int soundlength, soundpos;
Uint8 * sounddata;

/*===========================================================================*/
/*===========================================================================*/
void stop_audio_stream(AUDIOSTREAM *stream)
{
	printf("%s\n", __func__);
	SDL_PauseAudio(1);
	if (sounddata != NULL) {
		#ifdef __O2EM_MEM_DEBUG__
		printf("MALLOC/FREE DEBUG free sounddata\n");
		#endif
		free(sounddata);
		sounddata = NULL;
	}
	SDL_CloseAudio();
}

/*===========================================================================*/
/*===========================================================================*/
void mixaudio(void *userdata, Uint8 *stream, int len)
{
	/* Attention à ne pas déborder lors de la copie */
	Uint32 tocopy = soundlength - soundpos > len ? len : soundlength - soundpos; 
	if (stream == NULL) {
		printf("%s null\n", __func__);
	}
	#ifdef __O2EM_SDL_SOUND_DEBUG__
	printf("%s %d %d %d\n", __func__, soundlength, soundpos, len);
	#endif

	/* Copie des données sonores dans le tampon audio... */
/*	memcpy(stream, sounddata + soundpos, tocopy);*/
	SDL_MixAudio(stream, sounddata + soundpos, tocopy, SDL_MIX_MAXVOLUME);

	/* Mise à jour de la position de lecture */
	soundpos += tocopy;
	if (soundpos >= soundlength)
		SDL_PauseAudio(1);
}


/*===========================================================================*/
/*===========================================================================*/
AUDIOSTREAM *play_audio_stream(int len, int bits, int stereo, int freq, int vol, int pan)
{
	SDL_AudioSpec desired, obtained;
	size_t buff_size;

	desired.freq = freq;
	switch (bits) {
	case 8:
		desired.format = AUDIO_U8;
	break;
	default:
		printf("%s unknown bits %d\n", __func__, bits);
	}
	if (stereo == 1)
		desired.channels = 2;
	else
		desired.channels = 1;
	buff_size = len * desired.channels;
	desired.samples = len;/* SOUND_BUFFER_LEN ??*/
	desired.callback = &mixaudio;
	desired.userdata = NULL;
	printf("%s %d\n", __func__, len);
	sounddata = malloc(buff_size);
	#ifdef __O2EM_MEM_DEBUG__
	printf("MALLOC/FREE DEBUG malloc sounddata %ld\n", buff_size);
	#endif
	if (sounddata == NULL) {
		printf("%s malloc error %s\n", __func__, strerror(errno));
		return NULL;
	}
	memset(sounddata, 0, buff_size);
	soundpos = buff_size;
	soundlength = buff_size;

	if (SDL_OpenAudio(&desired, &obtained) != 0) {
		printf("%s SDL_OpenAudio error: %s\n", __func__, SDL_GetError());
		return NULL;
	}
	printf("Paramètres audio obtenus: %d canaux, fréquence %d échantillons %d\n", obtained.channels, obtained.freq, obtained.samples);
	SDL_PauseAudio(0);
	return (AUDIOSTREAM *)1;
}


/*===========================================================================*/
/*===========================================================================*/
void *get_audio_stream_buffer(AUDIOSTREAM *stream)
{
	if (soundlength - soundpos > 0) /* If it returns NULL, the stream is still playing*/
		return NULL;
	soundpos = 0;
	memset(sounddata, 0, soundlength);
	SDL_PauseAudio(1);
	return sounddata;
}

/*===========================================================================*/
/*===========================================================================*/
void free_audio_stream_buffer(AUDIOSTREAM *stream)
{
	/*printf("%s %p\n", __func__, (void *)stream);*/
	SDL_PauseAudio(0);
	soundpos = 0;
}


/*===========================================================================*/
/*===========================================================================*/
void voice_stop(int voice_num)
{
}
/*===========================================================================*/
/*===========================================================================*/
void voice_start(int voice_num)
{
}
/*===========================================================================*/
/*===========================================================================*/
int voice_get_position(int voice)
{
	return 0;
}
/*===========================================================================*/
/*===========================================================================*/
void voice_set_volume(int voice, int volume)
{
}

/*===========================================================================*/
/*===========================================================================*/
void reallocate_voice(int voice, const SAMPLE *spl)
{
}

/*===========================================================================*/
/*===========================================================================*/
int allocate_voice(const SAMPLE *spl)
{
	return 0;
}

/*===========================================================================*/
/*===========================================================================*/
SAMPLE *load_sample(const char *filename)
{
	if (filename == NULL) {
		fprintf(stderr, "%s error filanme is NULL\n", __func__);
		return NULL;
	}
	return NULL;
}

/*===========================================================================*/
/*===========================================================================*/
void destroy_sample(SAMPLE *spl)
{
	if (spl == NULL) {
		fprintf(stderr, "%s error spl is NULL\n", __func__);
		return;
	}
}

/*===========================================================================*/
/*===========================================================================*/
/* o2em use -1 as parameter so no change 
 * TODO remove set_volume ? */
void set_volume(int digi_volume, int midi_volume)
{
}




/*===========================================================================*/
/*===========================================================================*/
#endif/*O2EM_SDL*/

