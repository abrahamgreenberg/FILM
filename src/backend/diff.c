#include "diff.h"

void FormatDiffName(char *str, int number, const char *name)
{
    snprintf(str, OS_MAX_FILE_NAME_LENGTH, "[%02d] %s", number, name);
}

void DiffArrayConstructor(Diff **diffs, Folder *folders, size_t folders_size)
{
    Diff *diffArray = (Diff *)malloc(MAX_FOLDERS * sizeof(Diff));
    if (diffArray == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < folders_size; i++)
    {
        diffArray[i].index = i;
        diffArray[i].number = folders[i].number;
        diffArray[i].archive = 0;
        strcpy(diffArray[i].name, folders[i].name);
        strcpy(diffArray[i].formatted_name, folders[i].folder_name);
    }

    *diffs = diffArray;
}

void SwapDiffs(Diff *diffs, int i, int j)
{
    Diff temp = diffs[i];
    diffs[i] = diffs[j];
    diffs[j] = temp;

    diffs[j].number = diffs[i].number;
    diffs[i].number = temp.number;

    FormatDiffName(diffs[i].formatted_name, diffs[i].number, diffs[i].name);
    FormatDiffName(diffs[j].formatted_name, diffs[j].number, diffs[j].name);
}
