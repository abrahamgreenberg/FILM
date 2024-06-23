#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "opts.h"
#include "backend/folders.h"
#include "backend/diff.h"
#include "ui/ui.h"

/*
- ABILITY TO CREATE FOLDERS
- [A] TO ARCHIVE
    - AUTO CREATE ARCHIVE FOLDER
- OPTION TO RENUMBER SO INCREMENT (I.E REMOVE SKIPS)
- ADD VIM CRSOR MOVEMENT SUPPORT
- SETTINGS?
- BETTER UI
- ADD CONTROLS E.G:
RELOAD IN NAVIGATION VIEW
- ABILITY TO SELECT DIR
- LOOK INTO MAKE VS CLANG VS GCC AND MAKE A NICER DEVELOPMENT CONFIG
- BETTER CONTROLS SUPPORT
    - CUSTOM KEYBOARD SHORTCUTS
    - AUTO CREATE ARCHIVE FOLDER

 */

int main()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    Folder *folders = FolderArrayConstructor();
    if (folders == NULL)
        return 1;

    char render_path[MAX_PATH_LENGTH];
    char current_path[MAX_PATH_LENGTH];
    strcpy(current_path, getcwd(NULL, 0));

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

    Diff *diffs = NULL;

    while (1)
    {
        draw_loop(current_path, render_path, &diffs, folders, &folder_count, debug_string, &debug_string_length, message_string, &message_string_length, &start, max_level, &highlight, &edit, &write);
    }

    endwin();
    free(folders);
    if (diffs != NULL)
        free(diffs);
    return 0;
}
