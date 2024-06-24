#include "settings_controls.h"

void settings_controls(int ch, View *view)
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
            *view = NAVIGATE;
        break;
    }
}
