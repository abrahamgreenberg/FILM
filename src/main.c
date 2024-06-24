#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "opts.h"
#include "backend/folders.h"
#include "backend/diff.h"
#include "ui/ui.h"

/*
TODO:

FEATURES:
- LOOK INTO MAKE VS CLANG VS GCC AND MAKE A NICER DEVELOPMENT CONFIG
-BUGFIX: FOR SOME REASON WHEN CREATING A FOLDER IN A DIRECTORY WITH OTHER FILES, ARCHIVE IS SOMETIMES HIGHLIGHTED IN WRITE MODE, OTHER TIMES NOT
- REFACTOR:
1.  DIFF.ACTION
        1 FOR RENAME
        2 FOR CREATE
        4 FOR ARCHIVE
        8 FOR DELETE

MAKE A SETTINGS UI/SYSTEM:
- CUSTOM KEYBOARD SHORTCUTS
- AUTO CREATE ARCHIVE FOLDER
- COLOR THEMES

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

    int edit = 0;
    int write = 0;

    int help_page = 0;

    Diff *diffs = NULL;

    while (1)
    {
        draw_loop(current_path, render_path, &diffs, folders, &folder_count, debug_string, &debug_string_length, message_string, &message_string_length, &start, max_level, &highlight, &edit, &write, &help_page);
    }

    endwin();
    free(folders);
    if (diffs != NULL)
        free(diffs);
    return 0;
}
