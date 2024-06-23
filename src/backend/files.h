#ifndef FILES_H
#define FILES_H
#include "folders.h"
#include "diff.h"
#include "../ui/controls.h"
#include <dirent.h>

int path_level(const char *path);
void list_folders(const char *path, Folder *folders, int *folder_count, Diff **diffs);
void write_changes(const char *current_path, Folder *folders, Diff *diffs, size_t folders_count, char *message_string, int *message_string_length);

#endif