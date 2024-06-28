#include "save_settings.h"

char *settings_path(char *str)
{
    sprintf(str, "%s/.my_file_manager/settings.dat", getenv("HOME"));
}

void save_settings(Settings *settings)

{
    char path[OS_MAX_PATH_LENGTH];
    // mkdir("~/.my_file_manager", 0700);
    settings_path(path);
    FILE *file = fopen(path, "wb");
    if (file == NULL)
        return;

    fwrite(&settings->count, sizeof(int), 1, file);
    for (int i = 0; i < settings->count; i++)
    {
        Setting *setting = &settings->settings[i];
        int nameLen = strlen(setting->name) + 1;
        fwrite(&nameLen, sizeof(int), 1, file);
        fwrite(setting->name, sizeof(char), nameLen, file);
        fwrite(&setting->type, sizeof(SettingType), 1, file);

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
    char path[OS_MAX_PATH_LENGTH];
    settings_path(path);
    FILE *file = fopen(path, "rb");
    if (file == NULL)
        return;

    fread(&settings->count, sizeof(int), 1, file);
    settings->settings = (Setting *)malloc(settings->count * sizeof(Setting));

    for (int i = 0; i < settings->count; i++)
    {
        Setting *setting = &settings->settings[i];

        int nameLen;
        fread(&nameLen, sizeof(int), 1, file);

        setting->name = (char *)malloc(nameLen * sizeof(char));
        fread(setting->name, sizeof(char), nameLen, file);

        fread(&setting->type, sizeof(SettingType), 1, file);

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
