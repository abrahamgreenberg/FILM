#include "help.h"

/*
SHORTCUTS:
EXIT_KEY, NAVIGATE_KEY, RELOAD_DIRS_KEY, OPEN_EDIT_MODE_KEY, OPEN_WRITE_MODE_KEY,
OPEN_SETTINGS_KEY, OPEN_HELP_KEY, RENAME_KEY, MOVE_UP_KEY, MOVE_DOWN_KEY,
CREATE_DIR_KEY, ARCHIVE_DIR_KEY, DECREMENT_DIR_NUMBER_KEY, INCREMENT_DIR_NUMBER_KEY,
FIX_DIR_NUMBERS_KEY, CONFIRM_CHANGES_KEY, HIGHLIGHT_UP_KEY, HIGHLIGHT_DOWN_KEY
 */

const char *shortcutDescriptions[] = {
    "",
    "",
    "Quit",
    "Navigate",
    "Reload folders",
    "Open edit mode",
    "Save changes",
    "Settings",
    "Help",
    "Rename folder",
    "Move folder up",
    "Move folder down",
    "Create folder",
    "Archive folder",
    "Decrement folder number",
    "Increment folder number",
    "Fix folder numbers",
    "Confirm changes",
    "Move cursor up",
    "Move cursor down"};

/*
MODES:
NAVIGATE, EDIT, WRITE, SETTINGS, HELP,ALL VIEWS
*/

const int shortcutModeMappings[][9] = {
    {4, NAVIGATE_KEY, RELOAD_DIRS_KEY, OPEN_SETTINGS_KEY, OPEN_EDIT_MODE_KEY, 0, 0, 0, 0},
    {8, MOVE_UP_KEY, MOVE_DOWN_KEY, CREATE_DIR_KEY, CREATE_ARCHIVE, INCREMENT_DIR_NUMBER_KEY, DECREMENT_DIR_NUMBER_KEY, FIX_DIR_NUMBERS_KEY, OPEN_WRITE_MODE_KEY},
    {1, CONFIRM_CHANGES_KEY, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, HIGHLIGHT_UP_KEY, HIGHLIGHT_DOWN_KEY, EXIT_KEY, OPEN_HELP_KEY, 0, 0, 0, 0}};

void renderShortcutMappings(View view, int *j, Settings *settings)
{

    for (int i = 1; i < shortcutModeMappings[view][0]; i++)
    {
        char k[8];
        SettingIndexes index = shortcutModeMappings[view][i];
        setting_string(settings, index, k);
        mvprintw((*j)++, 0, "%s: %s\n", k, shortcutDescriptions[index]);
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

    renderShortcutMappings(view, &j, settings);
    renderShortcutMappings(VIEW_COUNT, &j, settings);

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