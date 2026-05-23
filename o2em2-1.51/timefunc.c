/*
 *   O2EM2 Free Odyssey2 / Videopac+ Emulator
 *
 *   Created by Daniel Boris <dboris@comcast.net>  (c) 1997, 1998
 *   Developed by Andre de la Rocha   <adlroc@users.sourceforge.net>
 *             Arlindo M. de Oliveira <dgtec@users.sourceforge.net>
 *
 *   Under development by LABBE Corentin http://o2em2.sourceforge.net
 *
 *
 *
 *   Timing functions
 */


#include <time.h>
#ifndef __O2EM_SDL__
	#include "allegro.h"
	#ifdef ALLEGRO_WINDOWS
		#include "winalleg.h"
		#include <sys/time.h>
		#include <time.h>
	#elif defined(ALLEGRO_UNIX) || defined(ALLEGRO_LINUX)
		#include <sys/time.h>
		#include <sys/times.h>
	#endif
#endif
#ifdef __O2EM_SDL__
	#include <SDL/SDL.h>
#endif
#include "timefunc.h"


#ifndef __O2EM_SDL__
#ifdef ALLEGRO_WINDOWS
static time_t first = 0;
/*============================================================================*/
/*============================================================================*/
/* Windows */
unsigned long gettimeticks()
{
	struct timeval tv;
	time_t ll;
	unsigned long ret;
	if (first == 0) {
		if (gettimeofday(&tv, NULL) != 0)
			return ((clock()) * TICKSPERSEC) / CLOCKS_PER_SEC;
		first = (tv.tv_sec) * 1000000 + tv.tv_usec;
		#ifdef __O2EM_TIME_DEBUG__
		printf("%s %lu\n", __func__, ll);
		#endif
	}
	if (gettimeofday(&tv, NULL) == 0) {
		ll = (tv.tv_sec) * 1000000 + tv.tv_usec;
		ret = ((ll - first) * TICKSPERSEC) / 1000000;
		#ifdef __O2EM_TIME_DEBUG__
		printf("%s %lu %lu\n", __func__, first, ll);
		#endif
		return ret;
	}
	return ((clock()) * TICKSPERSEC) / CLOCKS_PER_SEC;/* TODO*/
	/*
	LARGE_INTEGER counter, freq;
	static LONG_LONG first = -1;
	if (QueryPerformanceCounter(&counter))
		if (QueryPerformanceFrequency(&freq)) {
			if (first < 0)
				first = counter.QuadPart;
			return ((counter.QuadPart - first) * TICKSPERSEC) / freq.QuadPart;
		}
	return (((LONG_LONG)clock()) * TICKSPERSEC) / CLOCKS_PER_SEC;*/
}

#elif defined(ALLEGRO_UNIX) || defined(ALLEGRO_LINUX) /* ifdef ALLEGRO_WINDOWS */

#if defined(_BSD_SOURCE) || defined(FREEBSD)

static time_t first = 0;
/*============================================================================*/
/*============================================================================*/
/* Unix with gettimeofday (or LINUX) */

unsigned long gettimeticks()
{
	struct timeval tv;
	time_t ll;
	unsigned long ret;
	if (first == 0) {
		if (gettimeofday(&tv, NULL) != 0)
			return ((clock()) * TICKSPERSEC) / CLOCKS_PER_SEC;
		first = (tv.tv_sec) * 1000000 + tv.tv_usec;
	}
	if (gettimeofday(&tv, NULL) == 0) {
		ll = (tv.tv_sec) * 1000000 + tv.tv_usec;
		ret = ((ll - first) * TICKSPERSEC) / 1000000;
		return ret;
	}
	return ((clock()) * TICKSPERSEC) / CLOCKS_PER_SEC;
}

#else /* if defined(_BSD_SOURCE) || defined(FREEBSD) */

/*============================================================================*/
/*============================================================================*/
/* Unix without gettimeofday (TODO which unix for example ??)*/
unsigned long gettimeticks()
{
	struct tms t;
	return ((times(&t))*TICKSPERSEC)/sysconf(_SC_CLK_TCK);
}

#endif /* if defined(_BSD_SOURCE) || defined(FREEBSD) */

#else /* elif defined(ALLEGRO_UNIX) || defined(ALLEGRO_LINUX)*/

/* Default (use Allegro timers) */

volatile long ticks_counter = 0;
static int timer_init = 0;

/*============================================================================*/
/*============================================================================*/
void inc_ticks_counter()
{
	ticks_counter++;
}

END_OF_FUNCTION(inc_ticks_counter);

/*============================================================================*/
/*============================================================================*/

unsigned long gettimeticks()
{
	if (timer_init == 0) {
		ticks_counter = 0;
		LOCK_VARIABLE(ticks_counter);
		LOCK_FUNCTION(inc_ticks_counter);
		install_int(inc_ticks_counter, 1);
		timer_init = 1;
	}
	return ((LONG_LONG)ticks_counter * TICKSPERSEC) / 1000;
}

#endif
#else
/*============================================================================*/
/*============================================================================*/
unsigned long gettimeticks()
{
	return SDL_GetTicks() * 10;
}
#endif
