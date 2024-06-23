#include "folders.h"

Folder *FolderArrayConstructor()
{
    Folder *folderArray = (Folder *)malloc(MAX_FOLDERS * sizeof(Folder));
    if (folderArray == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (size_t i = 0; i < MAX_FOLDERS; ++i)
    {
        folderArray[i].number = 0;
        folderArray[i].name[0] = '\0';
        folderArray[i].folder_name[0] = '\0';
    }

    return folderArray;
}

void ModifyFolder(Folder *folderArray, int i, short number, const char *name, const char *folder_name)
{
    folderArray[i].number = number;
    strncpy(folderArray[i].name, name, MAX_FILE_NAME_LENGTH - 1);
    strncpy(folderArray[i].folder_name, folder_name, OS_MAX_FILE_NAME_LENGTH - 1);
}

int isValidFolderName(const char *line, short *number, char *name)
{
    if (line[0] == '[' && isdigit(line[1]) && isdigit(line[2]) && line[3] == ']' && isspace(line[4]))
    {
        char num_str[3];
        num_str[0] = line[1];
        num_str[1] = line[2];
        num_str[2] = '\0';

        *number = (short)strtol(num_str, NULL, 10);
        strncpy(name, line + 5, MAX_FILE_NAME_LENGTH - 1);
        return 1;
    }
    return 0;
}

int CompareFolders(const void *a, const void *b)
{
    const Folder *folderA = (const Folder *)a;
    const Folder *folderB = (const Folder *)b;

    return (folderA->number - folderB->number);
}
