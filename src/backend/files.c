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

IsDirReturnType is_dir(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) == 0)
        if (!S_ISDIR(sb.st_mode))
            return NOT_A_DIR;
        else
            return EXISTS;
    else
        return DOES_NOT_EXIST;
}

void list_folders(const char *path, Folder *folders, int *folder_count, Diff **diffs)
{
    DIR *d;
    struct dirent *dir;
    int temp_count = 0;

    short number;
    char name[MAX_FILE_NAME_LENGTH];
    char file_path[OS_MAX_PATH_LENGTH];

    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL && temp_count < MAX_FOLDERS)
        {
            snprintf(file_path, sizeof(file_path), "%s/%s", path, dir->d_name);

            IsDirReturnType isDir = is_dir(file_path);
            if (isDir != EXISTS)
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

void write_changes(const char *current_path, Folder *folders, Diff *diffs, size_t folders_count, char *message_string, int *message_string_length, Settings *settings)
{
    char original_path[MAX_PATH_LENGTH];
    char new_path[MAX_PATH_LENGTH];
    Diff diff;
    Folder folder;

    message_string[0] = '\0';
    *message_string_length = 0;

    for (int i = 0; i < (int)folders_count; i++)
    {
        diff = diffs[i];

        if (DiffHasAction(diff, CREATE))
        {
            snprintf(new_path, sizeof(new_path), "%s/%s", current_path, diff.formatted_name);

            if (mkdir(new_path, 0700))
                append_frmt(message_string, message_string_length, "(Failed to create folder %s) ", diff.formatted_name);

            if (settings->settings[CREATE_ARCHIVE].value.boolValue)
            {
                strncat(new_path, "/[99] Archive", sizeof(new_path));
                if (mkdir(new_path, 0700))
                    append_frmt(message_string, message_string_length, "(Failed to create folder %s/[99] Archive) ", diff.formatted_name);
            }
        }
        else if (DiffHasAction(diff, ARCHIVE) && !DiffHasAction(diff, CREATE))
        {
            snprintf(original_path, sizeof(new_path), "%s/%s", current_path, "[99] Archive");

            bool a = true;

            IsDirReturnType isDir = is_dir(original_path);
            if (isDir == NOT_A_DIR)
                a = false, append_str(message_string, message_string_length, "(A file by the name [99] Archive already exists) ");
            else if (isDir == DOES_NOT_EXIST)
                if (mkdir(original_path, 0700))
                    append_str(message_string, message_string_length, "(Failed to create folder [99] Archive) ");

            if (a)
            {
                folder = folders[diff.index];

                snprintf(original_path, sizeof(new_path), "%s/%s", current_path, folder.folder_name);
                snprintf(new_path, sizeof(new_path), "%s/[99] Archive/%s", current_path, diff.name);

                if (rename(original_path, new_path))
                    append_frmt(message_string, message_string_length, "(Failed to archive %s) ", diff.formatted_name);
            }
        }
        else if (DiffHasAction(diff, NAME))
        {
            folder = folders[diff.index];
            if (strcmp(folder.folder_name, diff.formatted_name) == 0)
                continue;

            snprintf(original_path, sizeof(new_path), "%s/%s", current_path, folder.folder_name);
            snprintf(new_path, sizeof(new_path), "%s/%s", current_path, diff.formatted_name);

            if (rename(original_path, new_path))
                append_frmt(message_string, message_string_length, "(Failed to rename %s to %s) ", folder.folder_name, diff.formatted_name);
        }
        append_str(message_string, message_string_length, "Done!");
    }
}