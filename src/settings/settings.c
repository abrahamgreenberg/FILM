#include "settings.h"

void init_setting_name(Setting *setting, const char *name)
{
    setting->name = (char *)malloc(STRING_LENGTH * sizeof(char));
    if (setting->name == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strcpy(setting->name, name);
}

Setting createBooleanSetting(const char *name, bool value)
{
    Setting setting;
    init_setting_name(&setting, name);
    setting.type = BOOLEAN;
    setting.value.boolValue = value;
    return setting;
}

Setting createColourSetting(const char *name, ColourTheme value)
{
    Setting setting;
    init_setting_name(&setting, name);
    setting.type = COLOUR;
    setting.value.colourValue = value;
    return setting;
}

Setting createShortcutSetting(const char *name, ColourTheme value)
{
    Setting setting;
    init_setting_name(&setting, name);
    setting.type = SHORTCUT;
    setting.value.charValue = value;
    return setting;
}

void init_settings(Settings *settings)
{
    const int settings_count = SETTINGS_COUNT;
    const int constant_settings = CONSTANT_SETTINGS;
    const int settings_total_count = SETTINGS_TOTAL_COUNT;
    *settings = (Setting *)malloc(SETTINGS_COUNT * sizeof(Setting));

    if (*settings == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    GET_SETTING(CREATE_ARCHIVE) = createBooleanSetting("Create archive", false);
    GET_SETTING(COLOUR_THEME) = createColourSetting("Colour Theme", DEFAULT_THEME);
    GET_SETTING(EXIT_KEY) = createShortcutSetting("Exit key", 'q');
    GET_SETTING(NAVIGATE_KEY) = createShortcutSetting("Navigate", 32);
    GET_SETTING(RELOAD_DIRS_KEY) = createShortcutSetting("Reload folders", 'r');
    GET_SETTING(OPEN_EDIT_MODE_KEY) = createShortcutSetting("Edit mode", 'e');
    GET_SETTING(OPEN_WRITE_MODE_KEY) = createShortcutSetting("Write mode", 'w');
    GET_SETTING(OPEN_SETTINGS_KEY) = createShortcutSetting("Settings", 's');
    GET_SETTING(OPEN_HELP_KEY) = createShortcutSetting("Help", 'h');
    GET_SETTING(RENAME_KEY) = createShortcutSetting("Rename", 'r');
    GET_SETTING(MOVE_UP_KEY) = createShortcutSetting("Move folder up", 'u');
    GET_SETTING(MOVE_DOWN_KEY) = createShortcutSetting("Move folder down", 'd');
    GET_SETTING(CREATE_DIR_KEY) = createShortcutSetting("Create folder", 'c');
    GET_SETTING(ARCHIVE_DIR_KEY) = createShortcutSetting("Archive", 'a');
    GET_SETTING(DECREMENT_DIR_NUMBER_KEY) = createShortcutSetting("Decrement folder number", '-');
    GET_SETTING(INCREMENT_DIR_NUMBER_KEY) = createShortcutSetting("Increment folder number", '=');
    GET_SETTING(FIX_DIR_NUMBERS_KEY) = createShortcutSetting("Fix folder numbers", 'f');
    GET_SETTING(CONFIRM_CHANGES_KEY) = createShortcutSetting("Confirm changes", 'c');
}

void free_settings(Settings *settings)
{
    if (*settings != NULL)
    {
        for (int i = 0; i < SETTINGS_COUNT; ++i)
        {
            free(GET_SETTING(i).name);
        }
        free(*settings);
    }
}