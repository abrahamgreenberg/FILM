#include "help.h"

void help_draw_ui(Settings *settings)
{
    clear();

    int j = 0;
    int c = GET_COLOUR(settings, CYAN);
    attron(c);
    mvprintw(j++, 0, "[Settings]");
    attroff(c);
    j += 2;

    refresh();
}

void help_menu_draw_loop(NAVIGATION_PARAMS(View *, int *, bool *, Settings *))
{
    help_draw_ui(settings);

    int ch = getch();
    if (ch >= 'A' && ch <= 'Z')
        ch += ('a' - 'A');

    if (ch == GET_SHORTCUT(EXIT_KEY))
    {
        navigate(CALL_NAVIGATE_PARAMS, *view);
    }
}