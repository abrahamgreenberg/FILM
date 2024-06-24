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

/*

TODO: MAKE A SETTINGS UI/SYSTEM:
- COLOR THEMES SETTINGS
- CUSTOM KEYBOARD SHORTCUTS
- AUTO CREATE ARCHIVE FOLDER

BUGFIX: MESSAGE IS SET MULTIPLE TIMES
BUGFIX: LIMIT MESSAGE CHARACTERS
BUGFIX: WHEN ARCHIVING A FOLDER THAT EXISTS IN THE ARCHIVE IT OVERIDES IT
BUGFIX: THERE MAY BE AN ISSUE WHEN ARCHIVING MULTIPLE FOLDERS, WRITING THEN CREATING A FOLDER (IT RENDERS INCORRECTLY IN THE EDIT VIEW)
BUGFIX: CREATING A FOLDER WHEN NUMBERS ARE OUT OF ORDER SEEMS TO BE A BIT BROKEN
    - MAY HAVE TO LOOK INTO SORTING EVERY TIME, ALTHOUGH THIS IS NOT IDEAL
TODO: ABILITY TO BRING IN FOLDERS THAT DON'T FOLLOW THE SYSTEM?

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
    initialise_settings(&settings);

    start_color();
    use_default_colors();

    init_pair(1, COLOR_MAGENTA, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_BLUE, -1);
    init_pair(4, COLOR_CYAN, -1);
    init_pair(5, COLOR_RED, -1);

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
        view = SETTINGS;
    }

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
