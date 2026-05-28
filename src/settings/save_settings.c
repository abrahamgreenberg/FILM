#include "save_settings.h"

void settings_dir_path(char *str)
{
    sprintf(str, "%s/.film", getenv("HOME"));
}

void settings_path(char *str)
{
    settings_dir_path(str);
    strcat(str, "/settings.dat");
}

void save_settings(Settings *settings)
{
    char path[OS_MAX_PATH_LENGTH];
    settings_dir_path(path);
    mkdir(path, 0700);
    settings_path(path);
    FILE *file = fopen(path, "wb");
    if (file == NULL)
        return;

    int settings_count = SETTINGS_COUNT;
    fwrite(&settings_count, sizeof(int), 1, file);
    for (int i = 0; i < SETTINGS_COUNT; i++)
    {
        Setting *setting = &GET_SETTING(i);

        switch (setting->type)
        {
        case BOOLEAN:
            fwrite(&setting->value.boolValue, sizeof(bool), 1, file);
            break;
        case COLOUR:
            fwrite(&setting->value.colourValue, sizeof(ColourTheme), 1, file);
            break;
        case SHORTCUT:
            fwrite(&setting->value.charValue, sizeof(char), 1, file);
            break;
        }
    }

    fclose(file);
}

void load_settings(Settings *settings)
{
    init_settings(settings);

    char path[OS_MAX_PATH_LENGTH];
    settings_path(path);
    FILE *file = fopen(path, "rb");
    if (file == NULL)
        return;

    int settings_count;
    fread(&settings_count, sizeof(int), 1, file);

    for (int i = 0; i < settings_count; i++)
    {
        Setting *setting = &GET_SETTING(i);

        switch (setting->type)
        {
        case BOOLEAN:
            fread(&setting->value.boolValue, sizeof(bool), 1, file);
            break;
        case COLOUR:
            fread(&setting->value.colourValue, sizeof(ColourTheme), 1, file);
            break;
        case SHORTCUT:
            fread(&setting->value.charValue, sizeof(char), 1, file);
            break;
        }
    }

    fclose(file);
}
