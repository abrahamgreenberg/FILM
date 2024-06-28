#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "main.h"
#include "opts.h"
#include "backend/folders.h"
#include "backend/diff.h"
#include "ui/ui.h"
#include "settings/settings.h"
#include "settings/settings_ui.h"
#include "settings/colours.h"
#include "settings/save_settings.h"

/*
[x] TODO: FINISH CUSTOM KEYBOARD SHORTCUTS
[ ] BUGFIX: CONVERT KEYBOARD SHORTCUTS TO LOWER CASE
[ ] BUGFIX: ADD -, = AND ENTER AS KEYBOARD SHORTCUTS
[ ] TODO: ADD PROMPT WHEN CHANGING KEYBOARD SHORTCUTS
[ ] TODO: WHEN SAVING SETTINGS MAKE SURE DIR EXISTS IF NOT CREATE IT

[ ] TODO: CURRENTLY HELP MENU DOESNT USE SHORTCUTS I WANT TO FIX THIS AND THE HELP UI IN GENERAL (MAYBE A HELP VIEW)
[ ] TODO: IMPROVE SETTINGS UI

[ ] TODO: ABILITY TO BRING IN FOLDERS THAT DON'T FOLLOW THE SYSTEM?

 */

int main(int argc, char *argv[])
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    if (!has_colors())
    {
        endwin();
        printf("Your terminal does not support colour\n");
        return 1;
    }

    Settings settings;
    init_settings(&settings);
    load_settings(&settings);

    start_color();
    use_default_colors();

    init_colours();

    Folder *folders = FolderArrayConstructor();
    if (folders == NULL)
        return 1;

    char render_path[MAX_PATH_LENGTH];
    char current_path[MAX_PATH_LENGTH];
    if (argc != 2)
        strcpy(current_path, getcwd(NULL, 0));
    else
    {
        IsDirReturnType isDir = is_dir(argv[1]);
        if (isDir != EXISTS)
        {
            printf("Invalid folder provided");
            return 1;
        }
        else
            strcpy(current_path, argv[1]);
    }

    char debug_string[STRING_LENGTH] = "";
    int debug_string_length = 0;

    char message_string[STRING_LENGTH] = "";
    int message_string_length = 0;

    int max_level = path_level(current_path);
    int start = 0;

    int folder_count = 0;
    int highlight = -1;

    int help_page = 0;
    Diff *diffs = NULL;
    View view = NAVIGATE;

    if (DEBUG_MODE == 1)
    {
        // view = NAVIAGE;
        // strcpy(current_path, "/home/abigreenberg/Documents/[01] Tech/[01] Programming/[02] c/file_manager/[01] test");
        view = SETTINGS;
        highlight = 2;
        // highlight = 4;
        // settings.settings[COLOUR_THEME].value.colourValue = 1;
    }

    bkgd(GET_COLOUR((&settings), BACKGROUND));

    while (1)
    {
        if (view != SETTINGS)
            file_manager_draw_loop(current_path, render_path, &diffs, folders, &folder_count, &settings, debug_string, &debug_string_length, message_string, &message_string_length, &start, max_level, &highlight, &view, &help_page);
        else
            settings_draw_loop(&view, &settings, &highlight);
    }

    endwin();
    free(folders);
    if (diffs != NULL)
        free(diffs);
    free_settings(&settings);
    return 0;
}
