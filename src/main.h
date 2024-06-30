#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>

typedef enum
{
    NAVIGATE,
    EDIT,
    WRITE,
    SETTINGS,
    HELP,
    VIEW_COUNT
} View;

#define NAVIGATION_PARAMS(VIEW_PRE, HL_PRE, HELP_PRE, SETTINGS_PRE) VIEW_PRE view, HL_PRE highlight, HELP_PRE help, SETTINGS_PRE settings
#define FOLDER_PARAMS(DIFF_PRE, FOLDERS_PRE, FOLDERS_COUNT_PRE) DIFF_PRE diffs, FOLDERS_PRE folders, FOLDERS_COUNT_PRE folder_count
#define PATH_PARAMS(PATH_PRE) PATH_PRE current_path, PATH_PRE render_path
#define DEBUG_PARAMS(STRING_PRE, LENGTH_PRE) STRING_PRE debug_string, LENGTH_PRE debug_string_length
#define MESSAGE_PARAMS(STRING_PRE, LENGTH_PRE) STRING_PRE message_string, LENGTH_PRE message_string_length

void navigate(View *view, int *highlight, bool *help, View navigateTo);
#define NAVIGATE_TO(T) navigate(view, highlight, help, T)

#endif