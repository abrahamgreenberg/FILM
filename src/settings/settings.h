#ifndef SETTINGS_H
#define SETTINGS_H
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../opts.h"

typedef enum
{
    BOOLEAN,
    COLOUR
} SettingType;

typedef struct
{
    char *name;
    SettingType type;
    union
    {
        bool boolValue;
        int colourValue;
    } value;
} Setting;

typedef struct
{
    Setting *settings;
    int count;
} Settings;

Setting createBooleanSetting(const char *name, bool value);
Setting createColourSetting(const char *name, int value);
void initialise_settings(Settings *settings);
void free_settings(Settings *settings);
#endif