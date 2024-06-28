#ifndef SETTINGS_CONTROLS_H
#define SETTINGS_CONTROLS_H
#include "../main.h"
#include "../opts.h"
#include "settings.h"
#include "colours.h"
#include <stdlib.h>
#include <ncurses.h>

void settings_controls(int ch, NAVIGATION_PARAMS(View *, int *, int *), Settings *settings);
#endif