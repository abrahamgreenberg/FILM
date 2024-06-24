#ifndef FILES_H
#define FILES_H
#include "folders.h"
#include "diff.h"
#include "../ui/controls.h"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef enum
{
    EXISTS,
    NOT_A_DIR,
    DOES_NOT_EXIST
} IsDirReturnType;

int path_level(const char *path);
void list_folders(const char *path, Folder *folders, int *folder_count, Diff **diffs);
void write_changes(const char *current_path, Folder *folders, Diff *diffs, size_t folders_count, char *message_string, int *message_string_length);
IsDirReturnType is_dir(const char *path);

#endif