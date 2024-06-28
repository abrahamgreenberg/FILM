#ifndef UI_H
#define UI_H

#include "../main.h"
#include "../opts.h"
#include "../backend/diff.h"
#include "../backend/folders.h"
#include "../backend/files.h"
#include "../settings/settings.h"
#include "../settings/colours.h"
#include "controls.h"
#include <ncurses.h>

void file_manager_draw_loop(NAVIGATION_PARAMS(View *, int *, int *, Settings *), FOLDER_PARAMS(Diff **, Folder *, int *), PATH_PARAMS(char *), DEBUG_PARAMS(char *, int *), MESSAGE_PARAMS(char *, int *), int *start, int max_level);

#endif