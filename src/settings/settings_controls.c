#include "settings_controls.h"

void settings_controls(int ch, View *view)
{
    switch (ch)
    {
    case 'q':
    case 'Q':
        *view = NAVIGATE;
    }
}
