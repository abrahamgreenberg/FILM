#ifndef SAVE_SETTINGS_H
#define SAVE_SETTINGS_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "settings.h"

void save_settings(Settings *settings);
void load_settings(Settings *settings);
#endif