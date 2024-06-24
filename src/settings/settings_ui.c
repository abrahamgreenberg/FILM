#include "settings_ui.h"

void settings_draw_ui(Settings *settings, int highlight)
{
    clear();

    int j = 0;
    mvprintw(j++, 0, "[Settings]");
    j += 2;

    char string_value[STRING_LENGTH];
    bool h;

    const char *ColourNames[] = {"Default", "Dracula", "Fancy"};

    for (int i = 0; i < settings->count * 2; i += 2)
    {
        h = highlight == i >> 1;

        mvprintw(j + i, 0, "%s", settings->settings[i >> 1].name);

        switch (settings->settings[i >> 1].type)
        {
        case BOOLEAN:
            strcpy(string_value, settings->settings[i >> 1].value.boolValue == false ? "[ ]" : "[*]");
            break;
        case COLOUR:
            strcpy(string_value,
                   ColourNames[settings->settings[i >> 1].value.colourValue]);
            break;
        }

        if (h)
            attron(A_STANDOUT);
        mvprintw(j + i + 1, 30, "%s", string_value);
        if (h)
            attroff(A_STANDOUT);
    }

    refresh();
}

void settings_draw_loop(View *view, Settings *settings, int *highlight)
{
    if (*highlight < 0)
        *highlight = 0;
    settings_draw_ui(settings, *highlight);

    int ch = getch();
    settings_controls(ch, view, settings, highlight);
}