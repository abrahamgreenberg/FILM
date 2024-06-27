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

Setting createShortcutSettings(const char *name, ColourTheme value)
{
    Setting setting;
    init_setting_name(&setting, name);
    setting.type = SHORTCUT;
    setting.value.charValue = value;
    return setting;
}

void init_settings(Settings *settings)
{
    settings->count = SETTINGS_COUNT;
    settings->settings = (Setting *)malloc(settings->count * sizeof(Setting));

    if (settings->settings == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    GET_SETTING(CREATE_ARCHIVE) = createBooleanSetting("Create archive", false);
    GET_SETTING(COLOUR_THEME) = createColourSetting("Colour Theme", DEFAULT_THEME);
    GET_SETTING(EXIT_KEY) = createShortcutSettings("Exit key", 'q');
}

void free_settings(Settings *settings)
{
    if (settings->settings != NULL)
    {
        for (int i = 0; i < settings->count; ++i)
        {
            free(GET_SETTING(i).name);
        }
        free(settings->settings);
    }
}