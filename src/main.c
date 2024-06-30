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


2.TODO: MAKE SETTINGS UI LOOK MORE LIKE HELP UI (I PREFER THE LOOK OF IT)

3. TODO: ADD "CHANGE SETTING" BUTTON

4. BUGFIX: ESCAPE HAS TO BE PRESSED TWICE WHEN BEING ENETRED AS A CHARACTER

5. BUGFIX: IF A NEW SETTING IS ADDED, CURRENLTY THEY ARE ALL OVERRIDDEN

6. TODO: MAKE navigate(CALL_NAVIGATE_PARAMS, WRITE); A MACRO INSTEAD OF CALL_NAVIGATE_PARAMS

FUTURE: TODO: ABILITY TO BRING IN FOLDERS THAT DON'T FOLLOW THE SYSTEM?

*/

void navigate(View *view, int *highlight, bool *help, View navigateTo)
{
    if (navigateTo == HELP || navigateTo == SETTINGS)
        *highlight = 0;
    else if (navigateTo == NAVIGATE)
        *highlight = -1;

    if (navigateTo == HELP)
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
    // load_settings(&settings);
    // FIXME:
    // if (settings.count != SETTINGS_COUNT)
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
        // view = WRITE;
        navigate(&view, &highlight, &help, SETTINGS);
        highlight = 0;
    }

    bkgd(COLOR_PAIR(COLOUR_INDEX((settings[COLOUR_THEME]).value.colourValue, BACKGROUND)));

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
