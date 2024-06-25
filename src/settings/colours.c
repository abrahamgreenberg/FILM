#include "colours.h"

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

    init_custom_color(DEFAULT_BACKGROUND_INDEX + 9, 0x000000);
    init_custom_color(DEFAULT_RED_INDEX + 9, 0xFF0000);
    init_custom_color(DEFAULT_BLUE_INDEX + 9, 0x0000FF);
    init_custom_color(DEFAULT_CYAN_INDEX + 9, 0x00FFFF);
    init_custom_color(DEFAULT_YELLOW_INDEX + 9, 0xFFFF00);
    init_custom_color(DEFAULT_MAGENTA_INDEX + 9, 0xFF00FF);
    init_custom_color(DEFAULT_TEXT_INDEX + 9, 0xffffff);

    init_custom_color(DRACULA_BACKGROUND_INDEX + 9, 0x282A36);
    init_custom_color(DRACULA_RED_INDEX + 9, 0xFF5555);
    init_custom_color(DRACULA_BLUE_INDEX + 9, 0x6272A4);
    init_custom_color(DRACULA_CYAN_INDEX + 9, 0x8BE9FD);
    init_custom_color(DRACULA_YELLOW_INDEX + 9, 0xF1FA8C);
    init_custom_color(DRACULA_MAGENTA_INDEX + 9, 0xFF79C6);
    init_custom_color(DRACULA_TEXT_INDEX + 9, 0xf8f8f2);

    init_custom_color(AYU_MIRAGE_BACKGROUND_INDEX + 9, 0x1F2430);
    init_custom_color(AYU_MIRAGE_RED_INDEX + 9, 0xF07178);
    init_custom_color(AYU_MIRAGE_BLUE_INDEX + 9, 0x39BAE6);
    init_custom_color(AYU_MIRAGE_CYAN_INDEX + 9, 0x95E6CB);
    init_custom_color(AYU_MIRAGE_YELLOW_INDEX + 9, 0xFFD580);
    init_custom_color(AYU_MIRAGE_MAGENTA_INDEX + 9, 0xE6B673);
    init_custom_color(AYU_MIRAGE_TEXT_INDEX + 9, 0xcbccc6);

    init_pair(DEFAULT_BACKGROUND_INDEX, DEFAULT_TEXT_INDEX + 9, DEFAULT_BACKGROUND_INDEX + 9);
    init_pair(DEFAULT_RED_INDEX, DEFAULT_RED_INDEX + 9, -1);
    init_pair(DEFAULT_BLUE_INDEX, DEFAULT_BLUE_INDEX + 9, -1);
    init_pair(DEFAULT_CYAN_INDEX, DEFAULT_CYAN_INDEX + 9, -1);
    init_pair(DEFAULT_YELLOW_INDEX, DEFAULT_YELLOW_INDEX + 9, -1);
    init_pair(DEFAULT_MAGENTA_INDEX, DEFAULT_MAGENTA_INDEX + 9, -1);

    init_pair(DRACULA_BACKGROUND_INDEX, DRACULA_TEXT_INDEX + 9, DRACULA_BACKGROUND_INDEX + 9);
    init_pair(DRACULA_RED_INDEX, DRACULA_RED_INDEX + 9, -1);
    init_pair(DRACULA_BLUE_INDEX, DRACULA_BLUE_INDEX + 9, -1);
    init_pair(DRACULA_CYAN_INDEX, DRACULA_CYAN_INDEX + 9, -1);
    init_pair(DRACULA_YELLOW_INDEX, DRACULA_YELLOW_INDEX + 9, -1);
    init_pair(DRACULA_MAGENTA_INDEX, DRACULA_MAGENTA_INDEX + 9, -1);

    init_pair(AYU_MIRAGE_BACKGROUND_INDEX, AYU_MIRAGE_TEXT_INDEX + 9, AYU_MIRAGE_BACKGROUND_INDEX + 9);
    init_pair(AYU_MIRAGE_RED_INDEX, AYU_MIRAGE_RED_INDEX + 9, -1);
    init_pair(AYU_MIRAGE_BLUE_INDEX, AYU_MIRAGE_BLUE_INDEX + 9, -1);
    init_pair(AYU_MIRAGE_CYAN_INDEX, AYU_MIRAGE_CYAN_INDEX + 9, -1);
    init_pair(AYU_MIRAGE_YELLOW_INDEX, AYU_MIRAGE_YELLOW_INDEX + 9, -1);
    init_pair(AYU_MIRAGE_MAGENTA_INDEX, AYU_MIRAGE_MAGENTA_INDEX + 9, -1);
}

void init_colour_themes(ColourThemeColours **colourThemes)
{
    *colourThemes = (ColourThemeColours *)malloc(sizeof(ColourThemeColours) * COLOUR_AMOUNT);
    if (*colourThemes == NULL)
    {
        printf("Could not allocate memory.\n");
        exit(1);
    }

    (*colourThemes)[DEFAULT_THEME].Background = DEFAULT_BACKGROUND_INDEX;
    (*colourThemes)[DEFAULT_THEME].Red = DEFAULT_RED_INDEX;
    (*colourThemes)[DEFAULT_THEME].Blue = DEFAULT_BLUE_INDEX;
    (*colourThemes)[DEFAULT_THEME].Cyan = DEFAULT_CYAN_INDEX;
    (*colourThemes)[DEFAULT_THEME].Yellow = DEFAULT_YELLOW_INDEX;
    (*colourThemes)[DEFAULT_THEME].Magenta = DEFAULT_MAGENTA_INDEX;

    (*colourThemes)[DRACULA_THEME].Background = DRACULA_BACKGROUND_INDEX;
    (*colourThemes)[DRACULA_THEME].Red = DRACULA_RED_INDEX;
    (*colourThemes)[DRACULA_THEME].Blue = DRACULA_BLUE_INDEX;
    (*colourThemes)[DRACULA_THEME].Cyan = DRACULA_CYAN_INDEX;
    (*colourThemes)[DRACULA_THEME].Yellow = DRACULA_YELLOW_INDEX;
    (*colourThemes)[DRACULA_THEME].Magenta = DRACULA_MAGENTA_INDEX;

    (*colourThemes)[AYU_MIRAGE_THEME].Background = AYU_MIRAGE_BACKGROUND_INDEX;
    (*colourThemes)[AYU_MIRAGE_THEME].Red = AYU_MIRAGE_RED_INDEX;
    (*colourThemes)[AYU_MIRAGE_THEME].Blue = AYU_MIRAGE_BLUE_INDEX;
    (*colourThemes)[AYU_MIRAGE_THEME].Cyan = AYU_MIRAGE_CYAN_INDEX;
    (*colourThemes)[AYU_MIRAGE_THEME].Yellow = AYU_MIRAGE_YELLOW_INDEX;
    (*colourThemes)[AYU_MIRAGE_THEME].Magenta = AYU_MIRAGE_MAGENTA_INDEX;
}

int get_colour(Settings *settings, ColourThemeColours *colourThemes, ColourThemeIndex)
{
    return COLOR_PAIR(colourThemes[settings->settings[COLOUR_THEME].value.colourValue].Background);
}