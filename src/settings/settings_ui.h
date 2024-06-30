#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H
#include <ncurses.h>
#include "../main.h"
#include "settings_controls.h"
#include "settings.h"
#include "colours.h"

void settings_draw_loop(NAVIGATION_PARAMS(View *, int *, bool *, Settings *));
void setting_string(Settings *settings, int i, char *s);

#endif