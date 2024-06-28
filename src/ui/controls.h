#ifndef CONTROLS_H
#define CONTROLS_H
#include "../main.h"
#include "../opts.h"
#include "../backend/folders.h"
#include "../backend/diff.h"
#include "../backend/files.h"
#include "../settings/settings.h"

#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdarg.h>

void append_str(char *string, int *string_length, const char *append_string);
void append_frmt(char *string, int *string_length, const char *format, ...);

void navigate_controls(int ch, NAVIGATION_PARAMS(View *, int *, int *), PATH_PARAMS(char *), Folder *folders, Settings *settings);
void edit_controls(int ch, FOLDER_PARAMS(Diff **, Folder *, int *), NAVIGATION_PARAMS(View *, int *, int *), DEBUG_PARAMS(char *, int *), Diff *diff, Settings *settings);
void write_controls(int ch, NAVIGATION_PARAMS(View *, int *, int *), FOLDER_PARAMS(Diff *, Folder *, int), MESSAGE_PARAMS(char *, int *), PATH_PARAMS(char *), Settings *settings);

#endif