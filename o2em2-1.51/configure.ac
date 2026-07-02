AC_INIT([o2em2], [1.51], [clabbe.montjoie@gmail.com])
AM_INIT_AUTOMAKE([-Wall foreign])

AC_PREREQ
AC_PROG_CC
AC_CONFIG_HEADERS([myconfig.h])

AC_FUNC_MALLOC

AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strerror])
AC_CHECK_FUNCS([strncasecmp])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_FUNCS([strtoul])
AC_CHECK_FUNCS([strcasecmp])

AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

CFLAGS="$CFLAGS -pipe -pedantic -g -Wall -fwrapv"

if test "${with_game_api}" = ""; then
   with_game_api=allegro
fi

#game_api_cflags=""
#game_api_libs=""
#game_api_libname=""

AC_ARG_WITH([game-api],
     [AS_HELP_STRING([--with-game-api],
     [use different game API. Available options: allegro, sdl [default=allegro]])],
     [],
     [game-api=allegro])
AC_ARG_WITH([mudflap],
     [AS_HELP_STRING([--with-mudflap],
     [Compile with mudflap [default=no]])],
     [],
     [mudflap=no])

case "${with_game_api}" in
     allegro)
#        AC_CHECK_LIB([alleg], [install_allegro],
#                     [game_api_cflags=`allegro-config --cflags`
#                      game_api_libs=`allegro-config --libs`
#                      game_api_libname="allegro"],
#                     [AC_MSG_ERROR([library 'allegro' is required.])],
#                     [`allegro-config --libs`])
#	AM_PATH_ALLEGRO()
	PKG_CHECK_MODULES(allegro, allegro < 5,
		[],
		[AC_MSG_FAILURE(allegro not found)])
#	CFLAGS="$CFLAGS `allegro-config --cflags`"
#	LIBS="$LIBS `allegro-config --libs`"
	CFLAGS="$CFLAGS $allegro_CFLAGS"
	LIBS="$LIBS $allegro_LIBS"
	;;
     allegro5)
#        AC_CHECK_LIB([alleg], [install_allegro],
#                     [game_api_cflags=`allegro-config --cflags`
#                      game_api_libs=`allegro-config --libs`
#                      game_api_libname="allegro"],
#                     [AC_MSG_ERROR([library 'allegro' is required.])],
#                     [`allegro-config --libs`])
#	CFLAGS="$CFLAGS `allegro-config --cflags`"
	LIBS="$LIBS `pkg-config --libs allegro_main-5.0`"
	LIBS="$LIBS `pkg-config --libs allegro_image-5.0`"
	LIBS="$LIBS `pkg-config --libs allegro_audio-5.0`"
	LIBS="$LIBS `pkg-config --libs allegro_ttf-5.0`"
        ;;
     sdl)
        AC_CHECK_LIB([SDL_image], [IMG_Load],
                     [
		     #game_api_cflags=`sdl-config --cflags`
#                      game_api_libs=`sdl-config --libs`
#                      game_api_libname="sdl"
#                      AC_DEFINE(__O2EM_SDL__, 1, [Define to 1 if you have 'SDL' library.])
			],
                     [AC_MSG_ERROR([library 'SDL' is required.])],
                     [`sdl-config --libs`])
	#check for SDL_gfx library
	AC_CHECK_LIB(SDL_gfx, rotozoomSurface, , AC_MSG_ERROR([
		*** Unable to find SDL_gfx libary (http://www.libsdl.org/projects/SDL_gfx/)
	]))
	AM_PATH_SDL()
	CFLAGS="$CFLAGS $SDL_CFLAGS -D__O2EM_SDL__"
	LIBS="$LIBS $SDL_LIBS `pkg-config SDL_gfx --libs`"
        ;;
         *)
          AC_MSG_ERROR([unknown game API backend])
        ;;
esac

case "${with_mudflap}" in
	yes)
	AC_CHECK_LIB(mudflapth, __mf_init, [
		CFLAGS="$CFLAGS -fmudflap -fmudflapth -funwind-tables"
		LDFLAGS="$LDFLAGS -lmudflapth -rdynamic"
		], [])
	;;
esac

AC_CONFIG_FILES([
Makefile
])
AC_OUTPUT
echo "******************************************************"
echo "CFLAGS:                        $CFLAGS"
echo "LDFLAGS:                       $LDFLAGS"
echo "LIBS:                          $LIBS"

