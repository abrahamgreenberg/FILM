#ifndef COLOURS_H
#define COLOURS_H
#include <ncurses.h>
#include <stdlib.h>
#include "settings.h"

typedef enum
{
    DEFAULT_BACKGROUND_INDEX = 1,
    DEFAULT_RED_INDEX,
    DEFAULT_BLUE_INDEX,
    DEFAULT_CYAN_INDEX,
    DEFAULT_YELLOW_INDEX,
    DEFAULT_MAGENTA_INDEX,
    DEFAULT_TEXT_INDEX,
    DRACULA_BACKGROUND_INDEX,
    DRACULA_RED_INDEX,
    DRACULA_BLUE_INDEX,
    DRACULA_CYAN_INDEX,
    DRACULA_YELLOW_INDEX,
    DRACULA_MAGENTA_INDEX,
    DRACULA_TEXT_INDEX,
    AYU_MIRAGE_BACKGROUND_INDEX,
    AYU_MIRAGE_RED_INDEX,
    AYU_MIRAGE_BLUE_INDEX,
    AYU_MIRAGE_CYAN_INDEX,
    AYU_MIRAGE_YELLOW_INDEX,
    AYU_MIRAGE_MAGENTA_INDEX,
    AYU_MIRAGE_TEXT_INDEX,
} ColourIndexes;

void init_custom_color(short color_number, int hex_value);
void init_colours();

typedef struct
{
    ColourIndexes Background;
    ColourIndexes Red;
    ColourIndexes Blue;
    ColourIndexes Cyan;
    ColourIndexes Yellow;
    ColourIndexes Magenta;
} ColourThemeColours;

typedef enum
{
    Background,
    Red,
    Blue,
    Cyan,
    Yellow,
    Magenta
} ColourThemeIndex;

void init_colour_themes(ColourThemeColours **colourThemes);
int get_colour(Settings *settings, ColourThemeColours *colourThemes, ColourThemeIndex);

#endif