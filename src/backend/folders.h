#ifndef FOLDERS_H
#define FOLDERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "../opts.h"

struct Folder
{
    short number;
    char name[MAX_FILE_NAME_LENGTH];
    char folder_name[OS_MAX_FILE_NAME_LENGTH];
};

typedef struct Folder Folder;

Folder *FolderArrayConstructor();
void ModifyFolder(Folder *folderArray, int i, short number, const char *name, const char *folder_name);
int isValidFolderName(const char *line, short *number, char *name);
int CompareFolders(const void *a, const void *b);

#endif