#include "colours.h"

#define INIT_COLOUR(T, C, H) (init_custom_color(COLOUR_NCURSES_INDEX(T, C), H))
#define INIT_THEME(THEME, HEX_CODES)                  \
    do                                                \
    {                                                 \
        INIT_COLOUR(THEME, BACKGROUND, HEX_CODES[0]); \
        INIT_COLOUR(THEME, RED, HEX_CODES[1]);        \
        INIT_COLOUR(THEME, BLUE, HEX_CODES[2]);       \
        INIT_COLOUR(THEME, CYAN, HEX_CODES[3]);       \
        INIT_COLOUR(THEME, YELLOW, HEX_CODES[4]);     \
        INIT_COLOUR(THEME, MAGENTA, HEX_CODES[5]);    \
        INIT_COLOUR(THEME, TEXT, HEX_CODES[6]);       \
    } while (0)

#define INIT_COLOUR_PAIR(T, I, C) (init_pair(COLOUR_INDEX(T, I), COLOUR_NCURSES_INDEX(T, C), COLOUR_NCURSES_INDEX(T, BACKGROUND)))
#define INIT_THEME_COLOUR_PAIRS(THEME)             \
    do                                             \
    {                                              \
        INIT_COLOUR_PAIR(THEME, BACKGROUND, TEXT); \
        INIT_COLOUR_PAIR(THEME, RED, RED);         \
        INIT_COLOUR_PAIR(THEME, BLUE, BLUE);       \
        INIT_COLOUR_PAIR(THEME, CYAN, CYAN);       \
        INIT_COLOUR_PAIR(THEME, YELLOW, YELLOW);   \
        INIT_COLOUR_PAIR(THEME, MAGENTA, MAGENTA); \
    } while (0)

void init_custom_color(short color_number, int hex_value)
{
    int r = (hex_value >> 16) & 0xFF;
    int g = (hex_value >> 8) & 0xFF;
    int b = hex_value & 0xFF;

    r = (r * 1000) / 255;
    g = (g * 1000) / 255;
    b = (b * 1000) / 255;

    init_color(color_number, r, g, b);
}

void init_colours()
{
    //    BACKGROUND, RED, BLUE, CYAN, YELLOW, MAGENTA, TEXT
    int default_theme_colors[] = {0x000000, 0xFF0000, 0x0000FF, 0x00FFFF, 0xFFFF00, 0xFF00FF, 0xffffff};
    int dracula_theme_colors[] = {0x282A36, 0xFF5555, 0x6272A4, 0x8BE9FD, 0xF1FA8C, 0xFF79C6, 0xf8f8f2};
    int ayu_mirage_theme_colors[] = {0x1F2430, 0xF07178, 0x39BAE6, 0x95E6CB, 0xFFD580, 0xE6B673, 0xcbccc6};

    INIT_THEME(DEFAULT_THEME, default_theme_colors);
    INIT_THEME(DRACULA_THEME, dracula_theme_colors);
    INIT_THEME(AYU_MIRAGE_THEME, ayu_mirage_theme_colors);

    INIT_THEME_COLOUR_PAIRS(DEFAULT_THEME);
    INIT_THEME_COLOUR_PAIRS(DRACULA_THEME);
    INIT_THEME_COLOUR_PAIRS(AYU_MIRAGE_THEME);
}