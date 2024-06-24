#ifndef SETTINGS_H
#define SETTINGS_H
#include <stdbool.h>

typedef enum
{
    Default
} ColourTheme;

struct Settings
{
    bool create_archive;
    ColourTheme colour_theme;
};

typedef struct Settings Settings;

void initialise_settings(Settings *settings);

#endif