#ifndef __CONFIG_H
#define __CONFIG_H

#define O2EM_VERSION "1.51"
#define RELEASE_DATE "(Apr/2013)"

#define ROM_O2 		1
#define ROM_G7400	2
#define ROM_C52 	3
#define ROM_JOPAC	4
#define ROM_UNKNOWN	99

#define O2EM_SUCCESS 0
#define O2EM_FAILURE -1

void o2em_clean_quit(int exitcode);



#define __O2EM_MEM_DEBUG__
#define __O2EM_KEYBOARD_DEBUG__
#define __O2EM_DEBUG__
#define __O2EM_VERBOSE__
#ifdef __O2EM_RELEASE__
#undef __O2EM_MEM_DEBUG__
#undef __O2EM_KEYBOARD_DEBUG__
#undef __O2EM_DEBUG__
#undef __O2EM_VERBOSE__
#endif

#endif

