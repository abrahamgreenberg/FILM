#include "files.h"

int path_level(const char *path)
{
    int count = 0;

    while (*path != '\0')
    {
        if (*path == '/')
            count++;
        path++;
    }

    return count;
}

void list_folders(const char *path, Folder *folders, int *folder_count, Diff **diffs)
{
    DIR *d;
    struct dirent *dir;
    char temp_folders[MAX_FOLDERS][OS_MAX_FILE_NAME_LENGTH];
    int temp_count = 0;

    short number;
    char name[MAX_FILE_NAME_LENGTH];

    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL && temp_count < MAX_FOLDERS)
        {
            if (dir->d_type != DT_DIR)
                continue;
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;
            if (isValidFolderName(dir->d_name, &number, name))
                ModifyFolder(folders, temp_count++, number, name, dir->d_name);
        }
        closedir(d);
    }

    qsort(folders, temp_count, sizeof(Folder), CompareFolders);
    DiffArrayConstructor(diffs, folders, temp_count);

    *folder_count = temp_count;
}

void write_changes(const char *current_path, Folder *folders, Diff *diffs, size_t folders_count, char *message_string, int *message_string_length)
{
    char original_path[MAX_PATH_LENGTH];
    char new_path[MAX_PATH_LENGTH];
    Diff diff;
    Folder folder;

    message_string[0] = '\0';
    *message_string_length = 0;

    for (int i = 0; i < folders_count; i++)
    {
        diff = diffs[i];
        if (diff.index != -1)
        {
            folder = folders[diff.index];
            if (strcmp(folder.folder_name, diff.formatted_name) == 0)
                continue;

            snprintf(original_path, sizeof(new_path), "%s/%s", current_path, folder.folder_name);

            snprintf(new_path, sizeof(new_path), "%s/%s", current_path, diff.formatted_name);

            int r = rename(original_path, new_path);

            if (r)
            {
                append_frmt(message_string, message_string_length, "(Failed to rename %s to %s) ", folder.folder_name, diff.formatted_name);
            }
        }
        else
        {
            snprintf(new_path, sizeof(new_path), "%s/%s", current_path, diff.formatted_name);

            // TODO: CREATE FOLDER
            int r = mkdir(new_path, 0700);
            if (r)
            {
                append_frmt(message_string, message_string_length, "(Failed to create folder %s) ", diff.formatted_name);
            }
        }
    }
    append_str(message_string, message_string_length, "Done!");
}
