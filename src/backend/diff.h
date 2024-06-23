
#ifndef DIFF_H
#define DIFF_H
#include "folders.h"
#include "../opts.h"
#include <stdbool.h>

struct Diff
{
    int index;
    short number;
    char name[MAX_FILE_NAME_LENGTH];
    char formatted_name[OS_MAX_FILE_NAME_LENGTH];
    bool archive;
};

typedef struct Diff Diff;

void DiffArrayConstructor(Diff **diffs, Folder *folders, size_t folders_size);
void SwapDiffs(Diff *diffs, int i, int j);
void FormatDiffName(char *str, int number, const char *name);

#endif
