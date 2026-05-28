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
    COLOUR_COUNT
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

typedef Setting *Settings;

typedef enum
{
    CREATE_ARCHIVE,
    COLOUR_THEME,
    EXIT_KEY,
    NAVIGATE_KEY,
    RELOAD_DIRS_KEY,
    OPEN_EDIT_MODE_KEY,
    OPEN_WRITE_MODE_KEY,
    OPEN_SETTINGS_KEY,
    OPEN_HELP_KEY,
    RENAME_KEY,
    MOVE_UP_KEY,
    MOVE_DOWN_KEY,
    CREATE_DIR_KEY,
    ARCHIVE_DIR_KEY,
    DECREMENT_DIR_NUMBER_KEY,
    INCREMENT_DIR_NUMBER_KEY,
    FIX_DIR_NUMBERS_KEY,
    CONFIRM_CHANGES_KEY,
    CHANGE_SETTING_KEY,
    HIGHLIGHT_UP_KEY,
    HIGHLIGHT_DOWN_KEY,
    TOTAL_SETTINGS_COUNT,
} SettingIndexes;

#define BASE_SETTINGS_COUNT 2
#define SETTINGS_COUNT (TOTAL_SETTINGS_COUNT - BASE_SETTINGS_COUNT)

Setting createBooleanSetting(const char *name, bool value);
Setting createColourSetting(const char *name, ColourTheme value);
void init_settings(Settings *settings);
void free_settings(Settings *settings);

#define GET_SETTING(I) ((*settings)[I])
#define GET_SETTING_VALUE(I) (GET_SETTING(I).value)
#define GET_SHORTCUT(I) (GET_SETTING_VALUE(I).charValue)
#endif