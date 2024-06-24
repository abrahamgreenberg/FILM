#ifndef UI_H
#define UI_H

#include "../main.h"
#include "../opts.h"
#include "../backend/diff.h"
#include "../backend/folders.h"
#include "../backend/files.h"
#include "controls.h"
#include <ncurses.h>

void file_manager_draw_loop(
    char *current_path,
    char *render_path,
    Diff **diffs,
    Folder *folders,
    int *folder_count,
    char *debug_string,
    int *debug_string_length,
    char *message_string,
    int *message_string_length,
    int *start,
    int max_level,
    int *highlight,
    View *view,
    int *help_page);

#endif