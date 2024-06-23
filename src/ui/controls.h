#ifndef CONTROLS_H
#define CONTROLS_H
#include "../opts.h"

#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdarg.h>

void append_str(char *string, int *string_length, const char *append_string);
void append_frmt(char *string, int *string_length, const char *format, ...);
int get_new_name(char *new_name);

#endif