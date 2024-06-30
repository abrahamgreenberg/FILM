#ifndef COLOURS_H
#define COLOURS_H
#include <ncurses.h>
#include <stdlib.h>
#include "settings.h"

typedef enum
{
    BACKGROUND,
    RED,
    BLUE,
    CYAN,
    YELLOW,
    MAGENTA,
    GREEN,
    TEXT,
    COLOUR_THEME_INDEX_AMOUNT
} ColourThemeIndex;

#define COLOUR_INDEX(T, C) ((T * COLOUR_THEME_INDEX_AMOUNT) + C + 1)
#define COLOUR_NCURSES_INDEX(T, C) (COLOUR_INDEX(T, C) + 9)

void init_custom_color(short color_number, int hex_value);
void init_colours();

#define GET_COLOUR(C) (COLOR_PAIR(COLOUR_INDEX(GET_SETTING_VALUE(COLOUR_THEME).colourValue, C)))

#endif