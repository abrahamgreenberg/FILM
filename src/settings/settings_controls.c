#include "settings_controls.h"
#include "save_settings.h"

void settings_controls(int ch, NAVIGATION_PARAMS(View *, int *, bool *, Settings *))
{
    if (ch == GET_SHORTCUT(EXIT_KEY))
        navigate(CALL_NAVIGATE_PARAMS, NAVIGATE);
    else if (ch == GET_SHORTCUT(OPEN_HELP_KEY))
        navigate(CALL_NAVIGATE_PARAMS, HELP);
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
        if (*highlight < SETTINGS_COUNT - 1)
            (*highlight)++;
        break;
    case ' ':
        if (GET_SETTING(*highlight).type == BOOLEAN)
            GET_SETTING_VALUE(*highlight).boolValue = !GET_SETTING_VALUE(*highlight).boolValue;
        else if (GET_SETTING(*highlight).type == COLOUR)
        {
            GET_SETTING_VALUE(*highlight).colourValue = (GET_SETTING_VALUE(*highlight).colourValue + 1) % COLOUR_AMOUNT;
            // GET_SETTING_VALUE(COLOUR_THEME)
            bkgd(GET_COLOUR(BACKGROUND));
        }
        else if (GET_SETTING(*highlight).type == SHORTCUT)
        {
            char c;
            bool valid = false;

            while (!valid)
            {
                mvprintw(LINES - 1, 0, "Enter new shortcut: ");
                refresh();

                c = getch();
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
                    valid = true;
                else if (c == 27 || c == 32 || c == 9 || c == 10)
                    valid = true;
                else if (c == '-' || c == '=')
                    valid = true;
                if (c >= 'A' && c <= 'Z')
                    c += ('a' - 'A');
            }
            GET_SETTING_VALUE(*highlight).charValue = c;
        }
        save_settings(settings);
        break;
    }
}
