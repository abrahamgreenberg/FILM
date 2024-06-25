#include "settings_controls.h"

// void update_colours(int index, ColourThemeColours *colourThemes)
// {
//     // int t = colourThemes[index].Background;
//     bkgd(COLOR_PAIR(colourThemes[index].Background));
// }

void settings_controls(int ch, View *view, Settings *settings, int *highlight, ColourThemeColours *colourThemes)
{
    switch (ch)
    {
    case 'q':
    case 'Q':
        *highlight = -1;
        *view = NAVIGATE;
        break;
    case KEY_UP:
    case 'k':
    case 'K':
        if (*highlight > 0)
            (*highlight)--;
        break;
    case KEY_DOWN:
    case 'j':
    case 'J':
        if (*highlight < settings->count - 1)
            (*highlight)++;
        break;
    case ' ':
        if (settings->settings[*highlight].type == BOOLEAN)
            settings->settings[*highlight].value.boolValue = !(settings->settings[*highlight].value.boolValue);
        else if (settings->settings[*highlight].type == COLOUR)
        {
            settings->settings[*highlight].value.colourValue = (settings->settings[*highlight].value.colourValue + 1) % COLOUR_AMOUNT;
            bkgd(get_colour(settings, colourThemes));
        }
        break;
    }
}
