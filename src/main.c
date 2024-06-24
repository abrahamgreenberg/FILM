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

MAKE A SETTINGS UI/SYSTEM:
- CUSTOM KEYBOARD SHORTCUTS
- AUTO CREATE ARCHIVE FOLDER
- MOVE FUNCTIONS TO CONTROLS.C
- COLOR THEMES

BUGFIX: MESSAGE IS SET MULTIPLE TIMES
BUGFIX: LIMIT MESSAGE CHARACTERS
BUGFIX: WHEN ARCHIVING A FOLDER THAT EXISTS IN THE ARCHIVE IT OVERIDES IT
TODO: ABILITY TO BRING IN FOLDERS THAT DON'T FOLLOW THE SYSTEM?
TODO: REPLACE WRITE AND EDIT WITH BOOLEAN OR MAKE IT AN ENUMr
TODO: REPLACE DIFF WITH A POINTER SO
(*DIFFS)[(HIGHLIGHT)] BECOMES *DIFF =

AND DIFF. BECOME *DIFF.

I THINK A BETTER COMPROMISE

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

    while (1)
    {
        if (view != SETTINGS)
            file_manager_draw_loop(current_path, render_path, &diffs, folders, &folder_count, debug_string, &debug_string_length, message_string, &message_string_length, &start, max_level, &highlight, &view, &help_page);
        else
            // mvprintw(0, 0, "hi");
            settings_draw_loop(&view);
    }

    endwin();
    free(folders);
    if (diffs != NULL)
        free(diffs);
    return 0;
}
