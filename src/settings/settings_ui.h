#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H
#include <ncurses.h>
#include "../main.h"
#include "settings_controls.h"
#include "settings.h"
#include "colours.h"

void settings_draw_loop(NAVIGATION_PARAMS(View *, int *, int *), Settings *settings);
#endif