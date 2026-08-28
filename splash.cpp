#include "menu.h"
#include "FrensHelpers.h"
#include <cstring>

static int fgcolorSplash = DEFAULT_FGCOLOR;
static int bgcolorSplash = DEFAULT_BGCOLOR;
void splash()
{
    char s[SCREEN_COLS + 1];
    ClearScreen(bgcolorSplash);

    strcpy(s, "Pico-PacPlus");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 2, s, fgcolorSplash, bgcolorSplash);

    strcpy(s, "Odyssey2/VideoPac+ for RP2040/2350");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 3, s, fgcolorSplash, bgcolorSplash);

    strcpy(s, "O2EM Emulator");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 5, s, fgcolorSplash, bgcolorSplash);
    strcpy(s, "Daniel Boris / Andre de la Rocha");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 6, s, CLIGHTBLUE, bgcolorSplash);
    strcpy(s, "LABBE Corentin");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 7, s, CLIGHTBLUE, bgcolorSplash);

#if !HSTX
    strcpy(s, "Pico DVI");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 9, s, fgcolorSplash, bgcolorSplash);
    strcpy(s, "@shuichi_takano");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 10, s, CLIGHTBLUE, bgcolorSplash);
#else
    strcpy(s, "Pico DVI____________HDMI Driver");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 9, s, fgcolorSplash, bgcolorSplash);
    strcpy(s, "@shuichi_takano_____fliperama86");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 10, s, CLIGHTBLUE, bgcolorSplash);
#endif

    strcpy(s, "Pico Port & SD Card Support");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 13, s, fgcolorSplash, bgcolorSplash);
    strcpy(s, "@frenskefrens");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 14, s, CLIGHTBLUE, bgcolorSplash);

    strcpy(s, "NES/WII controller support");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 17, s, fgcolorSplash, bgcolorSplash);
    strcpy(s, "@PaintYourDragon @adafruit");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 18, s, CLIGHTBLUE, bgcolorSplash);

    strcpy(s, "PCB Design:");
    putText(2, 21, s, fgcolorSplash, bgcolorSplash);
    strcpy(s, "@johnedgarpark DynaMight");
    putText(13, 21, s, CLIGHTBLUE, bgcolorSplash);

    strcpy(s, "https://github.com/");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 25, s, CLIGHTBLUE, bgcolorSplash);
    strcpy(s, "PicoPlus-devel/pico-pacPlus");
    putText(SCREEN_COLS / 2 - strlen(s) / 2, 26, s, CLIGHTBLUE, bgcolorSplash);
}
