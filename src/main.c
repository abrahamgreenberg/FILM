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

/*

TODO: MAKE A SETTINGS UI/SYSTEM:
- SAVE SETTINGS
- CUSTOM KEYBOARD SHORTCUTS

DONE: BUGFIX: WHEN ARCHIVING FOLDERS THEN CREATING FOLDERS AFTERWARDS THE NEW FOLDERS RENDERED IN THE EDIT VIEW ARE THE OLD FOLDERS. THIS IS NOT FIXED WHEN GOING BACK INTO THE NAVIGATE VIEW. THIS HIGHLIGHTS TWO ISSUES:
- FOLDERS NEED TO OVERRIDE PREVIOUS FOLDERS WHEN ARCHIVING THEN CREATING
- THE FOLDER COUNT IS NOT BEING RESET IF WE CREATE THEN DELETE FOLDERS
BUG
DONE: BUGFIX: CREATING NUMBERS ARE BROKEN:
- WHEN IN NORMAL DIR WITH ALL NORMAL FILES IT IS FINE
- WHEN JUST ARCHIVE IT CREATES ONE AFTER ARCHIVE
- WHEN CREATING FROM SCRATCH IT IS BROKEN
- WHEN CREATING ON THE ARCHIVE DIRECTORY IN NORMAL AMOUNT OF FILES, IT CREATES AN 01 AFTERWARDS:
    - NEED TO SAY IF THE CURRENT FOLDER NUMBER IS 99, THEN NUMBER = 99, HOWEVER IF FOLDERS AMOUNT IS 1, THEN CREATE 1
- NEED TO CHECK IF JUST ONE DIRECTORY THAT IS NOT [99] E.G JUST [4] IN THE FOLDER
    - THIS IS FINE SO IT APPEARS THAT JUST THE [99] IS A PROBLEM

BUGFIX: MESSAGE IS SET MULTIPLE TIMES - NEED TO REIMPLEMENT THE MESSAGE
BUGFIX: LIMIT MESSAGE CHARACTERS
BUGFIX: WHEN ARCHIVING A FOLDER THAT EXISTS IN THE ARCHIVE IT OVERIDES IT
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
    init_settings(&settings);

    start_color();
    use_default_colors();

    init_colours();

    ColourThemeColours *colourThemes;
    init_colour_themes(&colourThemes);

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
        strcpy(current_path, "/home/abigreenberg/Documents/[01] Tech/[01] Programming/[02] c/file_manager/[01] test");
        view = EDIT;
        highlight = 4;
        settings.settings[COLOUR_THEME].value.colourValue = 1;
    }

    bkgd(get_colour(&settings, colourThemes, BACKGROUND));

    while (1)
    {
        if (view != SETTINGS)
            file_manager_draw_loop(current_path, render_path, &diffs, folders, &folder_count, &settings, debug_string, &debug_string_length, message_string, &message_string_length, &start, max_level, &highlight, &view, &help_page, colourThemes);
        else
            settings_draw_loop(&view, &settings, &highlight, colourThemes);
    }

    endwin();
    free(folders);
    if (diffs != NULL)
        free(diffs);
    free_settings(&settings);
    return 0;
}
