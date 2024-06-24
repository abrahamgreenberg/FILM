#include "settings_ui.h"

void settings_draw_ui()
{
    clear();

    int j = 0;
    mvprintw(j++, 0, "[Settings]");

    refresh();
}

void settings_draw_loop(View *view)
{
    settings_draw_ui();

    int ch = getch();
    settings_controls(ch, view);
}