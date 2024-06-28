#include "settings_ui.h"

#define RENDER_KEY(I, S) \
    case I:              \
        strcpy(str, S);  \
        break;

void settings_draw_ui(Settings *settings, int highlight)
{
    clear();

    int j = 0;
    int c = GET_COLOUR(settings, YELLOW);
    attron(c);
    mvprintw(j++, 0, "[Settings]");
    attroff(c);
    j += 2;

    char string_value[STRING_LENGTH];
    bool h;

    const char *ColourNames[] = {"Default", "Dracula", "Ayu Mirage"};

    for (int i = 0; i < settings->count * 2; i += 2)
    {
        mvprintw(j + i, 0, "%s", GET_SETTING(i / 2).name);

        h = highlight == i >> 1;
        c = 0;
        switch (GET_SETTING(i / 2).type)
        {
        case BOOLEAN:
            bool b = GET_SETTING_VALUE(i / 2).boolValue;
            c = GET_COLOUR(settings, b ? MAGENTA : GREEN);

            strcpy(string_value, b ? "[*]" : "[ ]");
            break;
        case COLOUR:
            strcpy(string_value,
                   ColourNames[GET_SETTING_VALUE(i / 2).colourValue]);
            break;
        case SHORTCUT:
            char str[8], v;
            switch (v = GET_SETTING_VALUE(i / 2).charValue)
            {
                RENDER_KEY(27, "Escape")
                RENDER_KEY(32, "Space")
                RENDER_KEY(9, "Tab")
            default:
                str[0] = v;
                str[1] = '\0';
                break;
            }
            strcpy(string_value, str);
        }

        if (h)
            attron(A_STANDOUT);
        if (c != 0)
            attron(c);
        mvprintw(j + i + 1, 30, "%s", string_value);
        if (c != 0)
            attroff(c);
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
    if (ch >= 'A' && ch <= 'Z')
        ch += ('a' - 'A');

    settings_controls(ch, view, settings, highlight);
}