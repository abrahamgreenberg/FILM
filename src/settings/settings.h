#ifndef SETTINGS_H
#define SETTINGS_H
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../opts.h"

typedef enum
{
    DEFAULT_THEME,
    DRACULA_THEME,
    AYU_MIRAGE_THEME,
    COLOUR_AMOUNT
} ColourTheme;
typedef enum
{
    BOOLEAN,
    COLOUR,
    SHORTCUT
} SettingType;

typedef struct
{
    char *name;
    SettingType type;
    union
    {
        bool boolValue;
        ColourTheme colourValue;
        char charValue;
    } value;
} Setting;

typedef struct
{
    Setting *settings;
    int count;
} Settings;

typedef enum
{
    CREATE_ARCHIVE,
    COLOUR_THEME,
    EXIT_KEY,
    SETTINGS_COUNT
} SettingIndexes;

Setting createBooleanSetting(const char *name, bool value);
Setting createColourSetting(const char *name, ColourTheme value);
void init_settings(Settings *settings);
void free_settings(Settings *settings);

#define GET_SETTING(I) (settings->settings[I])
#define GET_SETTING_VALUE(I) (GET_SETTING(I).value)
#define GET_SHORTCUT(S) (GET_SETTING_VALUE(S).charValue)
#endif