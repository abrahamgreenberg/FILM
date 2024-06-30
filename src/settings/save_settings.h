#ifndef SAVE_SETTINGS_H
#define SAVE_SETTINGS_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "settings.h"

void save_settings(Settings *settings);
int load_settings(Settings *settings);
#endif