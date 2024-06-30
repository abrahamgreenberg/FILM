#include "settings_ui.h"

#define RENDER_KEY(I, S) \
    case I:              \
        strcpy(str, S);  \
        break;

void setting_string(Settings *settings, int i, char *s)
{
    char str[8], v;
    switch (v = GET_SETTING_VALUE(i).charValue)
    {
        RENDER_KEY(27, "Escape")
        RENDER_KEY(32, "Space")
        RENDER_KEY(9, "Tab")
        RENDER_KEY(10, "Enter")
    default:
        str[0] = v;
        str[1] = '\0';
        break;
    }
    strcpy(s, str);
}

void settings_draw_ui(Settings *settings, int highlight)
{
    clear();

    int j = 0;
    int c = GET_COLOUR(YELLOW);
    attron(c);
    mvprintw(j++, 0, "[Settings]");
    attroff(c);
    j += 2;

    char string_value[STRING_LENGTH];
    bool h;

    const char *ColourNames[] = {"Default", "Dracula", "Ayu Mirage"};

    for (int i = 0; i < SETTINGS_COUNT; i++)
    {
        mvprintw(j + i, 0, "%s", GET_SETTING(i).name);

        h = highlight == i;
        c = 0;
        switch (GET_SETTING(i).type)
        {
        case BOOLEAN:
            bool b = GET_SETTING_VALUE(i).boolValue;
            c = GET_COLOUR((b ? MAGENTA : GREEN));

            strcpy(string_value, b ? "[*]" : "[ ]");
            break;
        case COLOUR:
            c = GET_COLOUR(BLUE);
            strcpy(string_value,
                   ColourNames[GET_SETTING_VALUE(i).colourValue]);
            break;
        case SHORTCUT:
            c = GET_COLOUR(CYAN);
            setting_string(settings, i, string_value);
        }

        if (h)
            attron(A_STANDOUT);
        attron(c);
        mvprintw(j + i, 30, "%s", string_value);
        attroff(c);
        if (h)
            attroff(A_STANDOUT);
    }

    refresh();
}

void settings_draw_loop(NAVIGATION_PARAMS(View *, int *, bool *, Settings *))
{
    settings_draw_ui(settings, *highlight);

    int ch = getch();
    if (ch >= 'A' && ch <= 'Z')
        ch += ('a' - 'A');

    settings_controls(ch, NAVIGATION_PARAMS(, , , ));
}