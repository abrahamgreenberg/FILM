#include "help.h"

/*
MODES:
NAVIGATE, EDIT, WRITE, SETTINGS, HELP,ALL VIEWS
*/

const int shortcutModeMappings[][9] = {
    {4, NAVIGATE_KEY, RELOAD_DIRS_KEY, OPEN_SETTINGS_KEY, OPEN_EDIT_MODE_KEY, 0, 0, 0, 0},
    {8, MOVE_UP_KEY, MOVE_DOWN_KEY, CREATE_DIR_KEY, ARCHIVE_DIR_KEY, INCREMENT_DIR_NUMBER_KEY, DECREMENT_DIR_NUMBER_KEY, FIX_DIR_NUMBERS_KEY, OPEN_WRITE_MODE_KEY},
    {1, CONFIRM_CHANGES_KEY, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, HIGHLIGHT_UP_KEY, HIGHLIGHT_DOWN_KEY, EXIT_KEY, OPEN_HELP_KEY, 0, 0, 0, 0}};

void renderShortcutMappings(View view, int *j, Settings *settings)
{
    int c = GET_COLOUR(settings, YELLOW);
    char k[32];
    char d[STRING_LENGTH];

    for (int i = 1; i <= shortcutModeMappings[view][0]; i++)
    {
        SettingIndexes index = shortcutModeMappings[view][i];
        if (index == HIGHLIGHT_UP_KEY)
        {
            strcpy(k, "j/up arrow");
            strcpy(d, "Move cursor up");
        }
        else if (index == HIGHLIGHT_DOWN_KEY)
        {
            strcpy(k, "k/down arrow");
            strcpy(d, "Move cursor down");
        }
        else
        {
            setting_string(settings, index, k);
            strcpy(d, GET_SETTING(index).name);
        }
        attron(c);
        mvprintw(*j, 0, "%s: ", k);
        attroff(c);

        mvprintw((*j)++, strlen(k) + 2, "%s", d);
    }
}

void help_draw_ui(Settings *settings, View view)
{
    clear();

    int j = 0;
    int c = GET_COLOUR(settings, CYAN);
    attron(c);
    mvprintw(j++, 0, "[Help]");
    attroff(c);

    c = GET_COLOUR(settings, BLUE);
    attron(c);
    switch (view)
    {
    case NAVIGATE:
        mvprintw(j++, 0, "[Navigation mode]");
        break;
    case EDIT:
        mvprintw(j++, 0, "[Edit mode]");
        break;
    case WRITE:
        mvprintw(j++, 0, "[Write mode]");
        break;
    case SETTINGS:
        mvprintw(j++, 0, "[Settings]");
        break;
    default:
        break;
    }
    attroff(c);

    renderShortcutMappings(VIEW_COUNT, &j, settings);
    renderShortcutMappings(view, &j, settings);

    j += 2;

    refresh();
}

void help_menu_draw_loop(NAVIGATION_PARAMS(View *, int *, bool *, Settings *))
{
    help_draw_ui(settings, *view);

    int ch = getch();
    if (ch >= 'A' && ch <= 'Z')
        ch += ('a' - 'A');

    if (ch == GET_SHORTCUT(EXIT_KEY))
    {
        navigate(CALL_NAVIGATE_PARAMS, *view);
    }
}