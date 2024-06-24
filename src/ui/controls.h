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

void navigate_controls(int ch, int *highlight, char *current_path, char *render_path, Folder *folders, View *view);
void edit_controls(int ch, Diff *diff, Diff **diffs, int *highlight, Folder *folders, int *folder_count, View *view, char *debug_string, int *debug_string_length);
void write_controls(int ch, View *view, const char *current_path, Folder *folders, Diff *diffs, size_t folder_count, char *message_string, int *message_string_length, char *render_path, Settings *settings);

#endif