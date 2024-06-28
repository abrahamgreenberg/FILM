#include "settings_controls.h"
#include "save_settings.h"

void settings_controls(int ch, View *view, Settings *settings, int *highlight)
{
    if (ch == GET_SHORTCUT(EXIT_KEY))
    {
        *highlight = -1;
        *view = NAVIGATE;
    }
    switch (ch)
    {
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
        if (GET_SETTING(*highlight).type == BOOLEAN)
            GET_SETTING_VALUE(*highlight).boolValue = !GET_SETTING_VALUE(*highlight).boolValue;
        else if (GET_SETTING(*highlight).type == COLOUR)
        {
            GET_SETTING_VALUE(*highlight).colourValue = (GET_SETTING_VALUE(*highlight).colourValue + 1) % COLOUR_AMOUNT;
            bkgd(GET_COLOUR(settings, BACKGROUND));
        }
        else if (GET_SETTING(*highlight).type == SHORTCUT)
        {
            char c;
            bool valid = false;

            while (!valid)
            {
                c = getch();
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
                    valid = true;
                else if (c == 27 || c == 32 || c == 9)
                    valid = true;
            }
            GET_SETTING_VALUE(*highlight).charValue = c;
        }
        save_settings(settings);
        break;
    }
}
