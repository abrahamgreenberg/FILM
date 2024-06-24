#include "settings_controls.h"

void settings_controls(int ch, View *view, Settings *settings, int *highlight)
{
    switch (ch)
    {
    case 'q':
    case 'Q':
        if (DEBUG_MODE == 1)
        {
            endwin();
            exit(0);
        }
        else
            *highlight = -1;
        *view = NAVIGATE;
        break;
    case KEY_DOWN:
    case 'k':
    case 'K':
        if (*highlight > 0)
            (*highlight)--;
        break;
    case KEY_UP:
    case 'j':
    case 'J':
        if (*highlight < settings->count - 1)
            (*highlight)++;
        break;
    case ' ':
        if (settings->settings[*highlight].type == BOOLEAN)
            settings->settings[*highlight].value.boolValue = !(settings->settings[*highlight].value.boolValue);
        else if (settings->settings[*highlight].type == COLOUR)
            settings->settings[*highlight].value.colourValue = (settings->settings[*highlight].value.colourValue + 1) % COLOUR_AMOUNT;
        break;
    }
}
