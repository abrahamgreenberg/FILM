#include "settings_ui.h"

void settings_draw_ui(Settings *settings, int highlight)
{
    clear();

    int j = 0;
    mvprintw(j++, 0, "[Settings]");
    j += 2;

    for (int i = 0; i < settings->count; i++)
    {
        mvprintw(j + i, 0, "%s", settings->settings[i].name);
    }

    refresh();
}

void settings_draw_loop(View *view, Settings *settings, int *highlight)
{
    if (*highlight < 0)
        *highlight = 0;
    settings_draw_ui(settings, *highlight);

    int ch = getch();
    settings_controls(ch, view);
}