#ifndef SETTINGS_CONTROLS_H
#define SETTINGS_CONTROLS_H
#include "../main.h"
#include "../opts.h"
#include "settings.h"
#include <stdlib.h>
#include <ncurses.h>

void settings_controls(int ch, View *view, Settings *settings, int *highlight);
#endif