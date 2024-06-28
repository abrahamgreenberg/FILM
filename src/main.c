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
#include "ui/help.h"
#include "settings/settings.h"
#include "settings/settings_ui.h"
#include "settings/colours.h"
#include "settings/save_settings.h"

/*

TODO: CURRENTLY HELP MENU DOESNT USE SHORTCUTS I WANT TO FIX THIS AND THE HELP UI IN GENERAL (MAYBE A HELP VIEW)
TODO: HELP SHOULD BE A BOOLEAN, NOT A VIEW SO IT CAN BE ACTIVATED ANYWHERE
BUGFIX: ADD -, = AND ENTER AS KEYBOARD SHORTCUTS
BUGFIX: ESCAPE HAS TO BE PRESSED TWICE
TODO: IMPROVE SETTINGS UI
TODO: ABILITY TO BRING IN FOLDERS THAT DON'T FOLLOW THE SYSTEM?
BUGFIX: IF A NEW SETTING IS ADDED, CURRENLTY THEY ARE ALL OVERRIDDEN, NEED TO FIX THIS BEHAVIOUR

 */

void navigate(View *view, int *highlight, bool *help, View navigateTo)
{
    if (navigateTo == HELP || navigateTo == SETTINGS)
        *highlight = 0;
    else if (navigateTo == NAVIGATE)
        *highlight = -1;

    if (*help)
        *help = false;
    else if (navigateTo == HELP)
        *help = true;
    else
    {
        *help = false;
        *view = navigateTo;
    }
}

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
    load_settings(&settings);
    if (settings.count != SETTINGS_COUNT)
        init_settings(&settings);

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

    Diff *diffs = NULL;
    View view = NAVIGATE;
    bool help = false;

    if (DEBUG_MODE)
    {
        navigate(&view, &highlight, &help, HELP);
        highlight = 0;
    }

    bkgd(GET_COLOUR((&settings), BACKGROUND));

    while (1)
        if (help)
            help_menu_draw_loop(NAVIGATION_PARAMS(&, &, &, &));
        else
            switch (view)
            {
            case SETTINGS:
                settings_draw_loop(NAVIGATION_PARAMS(&, &, &, &));
                break;
            case NAVIGATE:
            case EDIT:
            case WRITE:
                file_manager_draw_loop(NAVIGATION_PARAMS(&, &, &, &), FOLDER_PARAMS(&, , &), PATH_PARAMS(), DEBUG_PARAMS(, &), MESSAGE_PARAMS(, &), &start, max_level);
                break;
            default:
                break;
            }

    endwin();
    free(folders);
    if (diffs != NULL)
        free(diffs);
    free_settings(&settings);
    return 0;
}
